#!/usr/bin/env python

import nose

from nose.tools import with_setup
from nose.tools import assert_true
from nose.tools import assert_equals

from mailutils import *


emails_for_tests = {'1': ('From MAILER-DAEMON Thu Jul 31 16:23:27 1997\r\n'
                          'From: =?utf-8?q?From=3A_admin=40ya=2Eru?= <admin@ya.ru>\r\nTo: mes@orazaev.ru\r\n'
                          'Subject: Please cleanup your home folder.\r\n\r\n'
                          'subj.\n-------\nYour admin.'),
                    '2': ('From MAILER-DAEMON Thu Jul 31 16:23:27 1997\r\n'
                          'From: =?utf-8?q?From=3A_sara=5Fconor=40skynet=2Ecom?= <sara_conor@skynet.com>\r\n'
                          'To: terminator@skynet.com\r\n'
                          'Subject: Important message\r\n\r\n'
                          'Please save the world.\nRegards Sara.')}


class FakeSMTP(smtplib.SMTP):
    def __init__(self, *args, **kwargs):
        pass

    def login(self, *args, **kwargs):
        return ('OK', 'Connected')

    def close(self):
        pass

    def sendmail(self, *args, **kwargs):
        pass


class FakeIMAP(imaplib.IMAP4):
    def __init__(self, *args, **kwargs):
        pass

    def login(self, *args, **kwargs):
        return ('OK', 'Connected')

    def close(self):
        pass

    def select(self, *args, **kwargs):
        return ('OK', 0)

    def search(self, *args, **kwargs):
        return ('OK', '1 2')

    def fetch(self, uid, *args, **kwargs):
        return ('OK', [(0, emails_for_tests[uid])])

    def store(self, *args, **kwargs):
        return ('OK', 0)


def patch_smtp_and_imap():
    smtplib._SMTP = smtplib.SMTP
    smtplib.SMTP = FakeSMTP
    imaplib._IMAP4 = imaplib.IMAP4
    imaplib.IMAP4 = FakeIMAP


def unpatch_smtp_and_imap():
    smtplib.SMTP = smtplib._SMTP
    imaplib.IMAP4 = imaplib._IMAP4
    delattr(smtplib, '_SMTP')
    delattr(imaplib, '_IMAP4')


@with_setup(patch_smtp_and_imap, unpatch_smtp_and_imap)
def test_Connection():
    conn = Connection()
    conn.authorize('username', 'password')
    for msg in conn.unseen_messages():
        assert_equals(str(msg.msg),
                      str(email.message_from_string(emails_for_tests[msg.uid])))
        assert_true(msg.subject() in ('Please cleanup your home folder.',
                                      'Important message'))
        assert_true(msg.get_from() in ('admin@ya.ru', 'sara_conor@skynet.com'))
    conn.close()


if __name__ == '__main__':
    nose.main(defaultTest='__main__')
