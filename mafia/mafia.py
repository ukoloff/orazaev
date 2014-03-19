#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import sys

from BeautifulSoup import BeautifulSoup

mapIdToName = {}

class FetchingStrategy(object):
    """
    Get pages from mafshow strategy.
    Use urllib2 fetcher for downloading pages.
    """

    @staticmethod
    def getGame(gid):
        """(int) -> str

        Fetch game with this game id.
        Returns page content.
        """

        url = urllib2.urlopen('http://mafshow.ru/games/out/{0}/'.format(gid))
        return url.read()



class BSParsingStrategy(object):
    """
    Stategy to parse games page on http://mafshow.ru.

    Uses BeautifulSoup for parsing.
    """

    @staticmethod
    def parse(gameRecord, page):
        """(MafiaGameRecord, str) -> NoneType
        """

        soup = BeautifulSoup(page)

        gameSoup = soup.findAll('div', attrs={'class':'all_table_game'})[0]
        BSParsingStrategy._parse(gameRecord, gameSoup)
        pass

    @staticmethod
    def _parse(gameRecord, gameSoup):
        """(MafiaGameRecord, BeautifulSoup) -> NoneType
        """

        gameTables = gameSoup.findAll('table')
        BSParsingStrategy._parseMeta(gameRecord, gameTables[1])
        BSParsingStrategy._parsePlayers(gameRecord, gameTables[3])
        BSParsingStrategy._parseRoles(gameRecord, gameTables[2])

    @staticmethod
    def _parseRoles(gameRecord, rolesTable):
        """(MafiaGameRecord, BeautifulSoup) -> NoneType)

        Parse roles of the game: sheriff, mafs and don.
        """

        roleRows = rolesTable.findAll('tr')
        donNo = int(roleRows[1].contents[1].renderContents())
        mafNo1 = int(roleRows[2].contents[1].renderContents())
        mafNo2 = int(roleRows[3].contents[1].renderContents())
        sheriffNo = int(roleRows[4].contents[1].renderContents())

        gameRecord.don = gameRecord.players[donNo - 1]
        gameRecord.mafs = (gameRecord.players[mafNo1 - 1], gameRecord.players[mafNo2 - 1])
        gameRecord.sheriff = gameRecord.players[sheriffNo - 1]

    @staticmethod
    def _parsePlayers(gameRecord, playersTable):
        """(MafiaGameRecord, BeautifulSoup) -> NoneType

        Fill players list. Order in list is their order
        on the table.
        """

        playersRows = playersTable.findAll('tr')
        gameRecord.players = []
        for i in xrange(10):
            try:
                id = int(playersRows[i + 1].contents[1].renderContents().strip())
            except:
                id = -1
            gameRecord.players.append(id)

            if not mapIdToName.has_key(id):
                nameSoup = playersRows[i + 1].contents[2]
                if nameSoup.findAll('a'):
                    mapIdToName[id] = nameSoup.contents[0].contents[0].strip()
                else:
                    mapIdToName[id] = nameSoup.renderContents().strip()

    @staticmethod
    def _parseMeta(gameRecord, metaTable):
        """(MafiaGameRecord, BeautifulSoup) -> NoneType

        Fill meta information about game by soup:
        judge, date and who wins.
        """

        metaRows = metaTable.findAll('tr')
        #print metaRows
        gameRecord.judge = metaRows[0].contents[1].renderContents().strip()
        gameRecord.date = metaRows[1].contents[1].renderContents().strip().split()[0]

        gameRecord.gid = int(metaRows[2].contents[1].renderContents().strip())
        wins = metaRows[3].contents[1].renderContents().strip()
        if wins == "Красные":
            gameRecord.wins = 'TRUE'
        elif wins == "Черные":
            gameRecord.wins = 'FALSE'
        else:
            print wins
            raise Exception("Unknown type of wins: '{0}'".format(wins))




class MafiaGameRecord(object):
    def __init__(self, gid, FetchStrategy=FetchingStrategy, ParseStrategy=BSParsingStrategy):
        """(MafiaGameRecord, int, FetchStrategy=FetchingStrategy, ParseStrategy=BSParsingStrategy) -> NoneType

        Parsing showtime mafia club games from urls
        like http://mafshow.ru/games/out/N/,
        where N is the number of a game.

        >>> print MafiaGameRecord(1).exportToCsv()
        1,"2012-02-18",TRUE,1,57,11,81,25,63,6,9,9993,28,9993,28,9,11,"Полторак Александр"
        >>> print MafiaGameRecord(5).exportToCsv()
        5,"2012-02-18",FALSE,9995,9993,17,28,32,126,9996,56,9997,9998,28,9993,17,126,"Полторак Александр"
        """

        page = FetchStrategy.getGame(gid)
        ParseStrategy.parse(self, page)
        print >> sys.stderr, self.__dict__

    def exportToCsv(self):
        """(MafiaGameRecord) -> str

        export game to csv string.
        """

        csv = '{0},"{1}",{2}'.format(self.gid, self.date, self.wins)
        csv += "," + ",".join((str(self.players[x]) for x in xrange(10)))
        csv += ",{0},{1},{2},{3}".format(self.don, self.mafs[0], self.mafs[1], self.sheriff)
        csv += ',"{0}"'.format(self.judge)
        return csv

    @staticmethod
    def csvHeader():
        """() -> str

        returns csv header for exportToCsv() funciton.
        """

        csv = '"GID","Date","RedWins"'
        csv += "," + ",".join(('"P{0}"'.format(x + 1) for x in xrange(10)))
        csv += ',"Don","Maf1","Maf2","Sheriff"'
        csv += ',"Judge"'
        return csv



if __name__ == '__main__':
    import doctest
    doctest.testmod()
