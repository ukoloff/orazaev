#!/usr/bin/env python
"""
:file: mailutils.py
:brief: Utils for work with mailbox.
"""

import email
import email.header
import imaplib
import smtplib
import logging
import time
import re

# Debugging is on.
# TODO: remove this.
logging.basicConfig(level=logging.DEBUG)


# Email constants:
UNSEEN = '(UNSEEN)'
RFC = '(RFC822)'
SEEN = '\\Seen'
SUBJECT = 'Subject'
FROM = 'From'
ADD_FLAG = '+FLAGS'
DEL_FLAG = '-FLAGS'

RE_EMAIL = re.compile(r'<[^@]+@[^@]+\.[^@]+>')


class BadReturnCode(Exception):
    pass


class Connection(object):
    """SMTP and IMAP4 connection wrapper."""

    def __init__(self, smtp_server='smtp.yandex.ru', imap_server='imap.yandex.ru'):
        """Create not authorized Connection.

           :param smtp_server: link to smtp server (default: smtp.yandex.ru)
           :param imap_server: link to imap server (default: imap.yandex.ru)
        """
        self.smtp_server = smtp_server
        self.imap_server = imap_server

        self.smtp = smtplib.SMTP(smtp_server)
        self.imap = imaplib.IMAP4(imap_server)
        self._user = None
        self._passwd = None

    def authorize(self, usr, passwd, folder='Inbox'):
        """Authorize smtp and imap connection using login.

           :param usr:    username.
           :param passwd: password for current user.
           :param folder: selected folder by imap. (default: 'Inbox')

           .. TODO::
              * Use SSL authorization instead of login.
              * Check is logged in?
        """
        logging.info("Login for user {0}".format(usr))
        logging.debug("Login to smtp: {0}".format(self.smtp.login(usr, passwd)))
        logging.debug("Login to imap: {0}".format(self.imap.login(usr, passwd)))
        self._user = usr
        self._passwd = passwd
        logging.debug("Select folder '{0}': {1}".format(folder, self.imap.select(folder)))

    def _reconnect(self):
        """Reconnect to smtp and imap server if they are unexpectly disconnected."""
        if self._user is None or self._passwd is None:
            raise Exception("Cannot reconnect to servers. You should authorize at first.")
        logging.info("Reconnecting...")
        self.authorize(self._user, self._passwd)

    def close(self):
        """Close imap and smtp connection."""
        self.imap.close()
        self.smtp.close()

    def unseen_messages(self, mark_as_seen=False):
        """Get generator of Message.

           :param mark_as_seen: if it is True, then mark emails as seen after fetching.

           :returns: generator of Message from current imap4 connection
           from selected folder.
        """
        retcode, messages = self.imap.search(None, UNSEEN)
        if retcode != 'OK':
            raise BadReturnCode(retcode)

        for email_uid in messages[0].split(' '):
            logging.info('Processing: {0}'.format(email_uid))
            ret, data = self.imap.fetch(email_uid, RFC)
            msg = email.message_from_string(data[0][1])

            if mark_as_seen:
                ret, data = self.imap.store(email_uid, ADD_FLAG, SEEN)

            if ret != 'OK':
                logging.error('Can\'t add {0} flag to {1} message.'.format(SEEN, email_uid))
            else:
                yield Message(email_uid, msg)

    def sendmail(self, sendfrom, sendto, msg, subj=None, max_tries=3):
        """Send message to given address.

           :param sendfrom:  send mail from given address.
           :param sendto:    send to message to given email.
           :param msg:       message body.
           :param subject:   message subject.
           :param max_tries: maximal retries number if sendmail is fails.
        """
        email_msg = ''
        email_msg += "TO: {0}\r\n".format(sendto)
        email_msg += "FROM: {0}\r\n".format(sendfrom if sendfrom is not None else self.user)
        if subj is not None:
            email_msg += "SUBJECT: {0}\r\n".format(subj)
        email_msg += "\r\n{0}".format(msg)

        logging.debug('Sending message to {0} from {1}: {2}'\
                      .format(sendto, sendfrom, repr(email_msg)))

        retry_to_call(lambda: self.smtp.sendmail(sendfrom, sendto, email_msg),
                      repair=lambda: self._reconnect())

    def noop(self):
        """Send noop to imap server and to smtp server.

           If one of connection is dead then trying to reconnect.
        """
        def noop_imap():
            retcode, data = self.imap.noop()
            if ret != 'OK': raise BadReturnCode(retcode)

        def noop_smtp():
            retcode, data = self.smtp:noop()
            if ret != 250: raise BadReturnCode(retcode)

        retry_to_call(noop_imap, sleep=1, repair=lambda: self._reconnect())
        retry_to_call(noop_smtp, sleep=1, repair=lambda: self._reconnect())

    def __enter__(self):
        pass

    def __exit__(self, typ, value, tb):
        self.close()


class Message(object):
    """Helper class. Wrapper around email.message.Message and uid.

       .. todo::
          * Refactor functions like mark_seen to this class.
    """

    def __init__(self, uid, msg):
        """Create message with given uid and email.message.Message."""
        self.uid = uid
        self.msg = msg

    def subject(self):
        """(Message) -> str

           :returns: emails message decoded email subject.
        """
        return email.header.decode_header(self.msg.get(SUBJECT))[0][0]

    def get_from(self):
        """(Message) -> str or None

           :returns: decoded sender email address; or None if
           there are no FROM header in email.
        """
        for data, coding in email.header.decode_header(self.msg.get(FROM)):
            if RE_EMAIL.match(data):
                return data[1:-1]

        return None

    def mark_seen(self, connection):
        """(Message, Connection) -> None

           Add Seen flag for this message for @connection.
        """
        logging.info("Mark as unseen {0}: {1}"\
                     .format(uid, connection.imap.store(self.uid, ADD_FLAG, SEEN)))

    def mark_unseen(self, connection):
        """(Message, Connection) -> None

           Remove Seen flag for this message for @connection.
        """
        logging.info("Mark as unseen {0}: {1}"\
                 .format(uid, connection.imap.store(self.uid, DEL_FLAG, SEEN)))


def authorize():
    """() -> Connection

       Small script for authorization.

       :returns: authorized connection.
    """
    conn = Connection()

    from getpass import getpass
    usr = raw_input('Login: ')
    passwd = getpass('Password: ')
    conn.authorize(usr, passwd)

    return conn


def retry_to_call(func, max_retries=5, sleep=5, repair=None):
    """Retry to call funciton several times if it fails.

       :param func: function to call.
       :param max_retries: maximumal number of retries (default: 5).
       :param sleep: sleep interval between function calls (default: 5).
       :param repair: call this function if @func fails (default: None).
    """
    for trie in xrange(max_retries):
        try:
            return func()
        except:
            logging.exception("retry_to_call fails...")
            if trie == max_retries - 1:
                raise Exception("Function fails {0} times, giving up..."\
                                .format(max_retries))
            time.sleep(sleep)
            if repair is not None:
                repair()


if __name__ == '__main__':
    pass
