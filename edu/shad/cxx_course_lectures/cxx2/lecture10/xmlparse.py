#!/usr/bin/env python

import xml.sax

class MyHandler(xml.sax.handler.ContentHandler):
    def __init__(self):
        """(MyHandler) -> NoneType"""

        self.lvl = 0

    def startElement(self, name, attrs):
        """(MyHandler, str, xml.sax.xmlreader.AttributeImpl) -> NoneType"""

        if name == 'ana' and self.lvl > 0:
            try:
                print attrs['gr']
            except KeyError:
                pass
        elif name == 'w':
            self.lvl += 1

    def endElement(self, name):
        """(MyHandler, str) -> NoneType"""

        if name == 'w':
            self.lvl -= 1



if __name__ == '__main__':
    xml.sax.parse('file.xml', MyHandler())
