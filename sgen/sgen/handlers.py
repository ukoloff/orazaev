#!/usr/bin/env python
"""
Handler packs for blocks definitions.
"""

import blocks

class HandlerPack:
    """TODO"""

    def DummyHandler(self, data):
        """(HandlerPack, str) -> str

           Dummy block handler.
           Do nothing, just return empty string.
        """

        return ""

    def TextHandler(self, data):
        """(HandlerPack) -> str

           returns data.
        """

        return data


class CxxHandlerPack(HandlerPack):
    """TODO"""

    _mapBlockToHandler = {
        blocks.SourceFileBlock : DummyHandler,
        blocks.CommentBlock : CxxCommentHandler,
        blocks.RawTextBlock : TextHandler
    }
