#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import requests
import pyquery
import os
import sys
import itertools


SHIYANBIN_PAGE = 'http://shiyanbin.ru'
SHIYANBIN_EVENTS_PAGE = SHIYANBIN_PAGE + '/events'
SHIYANBIN_ALBUM_LINK_CSS_CLASS = '.gallery-collection-item-link'
SHIYANBIN_IMG_LINK_CSS_CLASS = '.gallery-collection-item-link'

REQUEST_TIMEOUT=120


def parse_args():
    parser = argparse.ArgumentParser(
        description='tool for fetching albums from shiyanbin')

    parser.add_argument('outdir')
    parser.add_argument('-e', '--events-page', default=SHIYANBIN_EVENTS_PAGE,
                        help='web page with albums')
    return parser.parse_args()


class CantFetchLinkError(BaseException):
    def __init__(self, href):
        super(BaseException, self).__init__()
        self.message = "Can't fetch {0} page".format(href)


def retry_get(href, times=3, not_fail_404=False):
    sys.stdout.write('DEBUG: Downloading {0}\n'.format(href))
    sys.stdout.flush()
    tries = 0
    while tries < times:
        tries += 1
        try:
            response = requests.get(href, timeout=REQUEST_TIMEOUT)
        except requests.exceptions.Timeout:
            sys.stdout.write('ERROR: timeout\n')
            sys.stdout.flush();
            continue
        except requests.exceptions.ChunkedEncodingError:
            sys.stdout.write('ERROR: chunked encoding error\n')
            sys.stdout.flush();
            continue
        if response.ok or not_fail_404 and response.status_code == 404:
            return response
    raise CantFetchLinkError(href)


def save_images_to_directory(directory, image_links):
    directory = directory.replace('"', "'").replace(':', '-')
    sys.stdout.write('INFO: Fetching directory {0}\n'.format(directory))
    if not os.path.exists(directory):
        os.makedirs(directory)
    already_fetched = len(os.listdir(directory))
    img_counter = itertools.count()
    for link in image_links:
        img_id = img_counter.next()
        if already_fetched > img_id:
            continue
        response = retry_get(link, not_fail_404=True)
        if response.status_code == 404:
            sys.stdout.write("ERROR: can't fetch {0}\n".format(link))
            continue
        with open(os.path.join(directory, '{0}.jpg'.format(img_id)), 'wb') as img_file:
            sys.stdout.write("DEBUG: saving to {0}\n".format(img_file.name))
            img_file.write(response.content)


def fetch_albums_from_shiyanbin(outdir, events_page):
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    response = retry_get(events_page)

    pq = pyquery.PyQuery(response.content)
    for album in pq(SHIYANBIN_ALBUM_LINK_CSS_CLASS):
        response = retry_get(SHIYANBIN_PAGE + album.attrib['href'])
        q = pyquery.PyQuery(response.content)
        save_images_to_directory(
            os.path.join(outdir, q('title').text().encode('utf-8')),
            (e.attrib['href'] for e in q(SHIYANBIN_IMG_LINK_CSS_CLASS)))




if __name__ == '__main__':
    args = parse_args()
    fetch_albums_from_shiyanbin(args.outdir, args.events_page)
