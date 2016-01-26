#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import pyquery
import requests
import json
import itertools
import logging
import re

from BeautifulSoup import BeautifulSoup
from glicko2 import play, Player

mapIdToName = {}


MAFSHOW_GAMES_PAGE = 'http://mafshow.ru/games/?page={0}'
MAFSHOW_PLAYER_PAGE = 'http://mafshow.ru/id{0}'
RE_MAFSHOW_GAME_HREF = re.compile('^/games/out/([^/]*)/?$')


class ParsingError(Exception):
    pass


def getGameIds():
    """
    Yields all game ids on mafshow.ru site
    """
    for page in itertools.count(1):
        response = requests.get(MAFSHOW_GAMES_PAGE.format(page))
        logging.info('fetching page: {}'.format(MAFSHOW_GAMES_PAGE.format(page)))
        pq = pyquery.PyQuery(response.content)
        game_ids = [
            RE_MAFSHOW_GAME_HREF.match(href).groups()[0]
            for href in (a.attrib.get('href', '') for a in pq('a'))
            if RE_MAFSHOW_GAME_HREF.match(href)]
        if len(game_ids) == 0:
            break
        for game_id in game_ids:
            yield game_id.encode('utf-8') if isinstance(game_id, unicode) else game_id


class MafiaPlayer(object):
    """
    >>> selensky = MafiaPlayer(777)
    >>> print selensky.name
    Роман SeLensky Селенский
    >>> selensky.player_id
    777
    >>> print selensky.toJson()
    {"id": 777, "name": "Роман SeLensky Селенский", "rating": [1200.0, 350.0]}
    """
    def __init__(self, player_id, rating=None):
        self._player_id = player_id
        self._fetch_and_init()
        if rating is None:
            self._rating = Player()
        else:
            self._rating = Player(rating[0], rating[1])

    def update_player(self, *args):
        self._rating.update_player(*args)

    def getRating(self):
        return self._rating.getRating()

    def getRd(self):
        return self._rating.getRd()

    def _fetch_and_init(self):
        response = requests.get(MAFSHOW_PLAYER_PAGE.format(self._player_id))
        pq = pyquery.PyQuery(response.content)
        self._name = pq('.person > h1')[0].text_content().strip().encode('utf-8')

    @property
    def rating(self):
        return (self._rating.getRating(), self._rating.getRd())

    @property
    def name(self):
        return self._name

    @property
    def player_id(self):
        return self._player_id

    def toJson(self):
        return '{{"id": {id}, "name": "{name}", "rating": {rating}}}'.format(
            id=self.player_id,
            name=self.name,
            rating=list(self.rating))


class FetchingStrategy(object):
    """
    Get pages from mafshow strategy.
    Use requests for downloading pages.
    """

    @staticmethod
    def getGame(gid):
        """(str) -> str

        Fetch game with this game id.
        Returns page content.
        """
        return requests.get('http://mafshow.ru/games/out/{0}/'.format(gid)).content



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
        try:
            BSParsingStrategy._parseRoles(gameRecord, gameTables[2])
        except Exception as e:
            raise ParsingError('Can\'t parse roles: ' +  str(e))

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

        gameRecord.gid = metaRows[2].contents[1].renderContents().strip()
        wins = metaRows[3].contents[1].renderContents().strip()
        if wins == "Красные":
            gameRecord.wins = True
        elif wins == "Черные":
            gameRecord.wins = False
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
        1,"2012-02-18",True,1,57,11,81,25,63,6,9,9993,28,9993,28,9,11,"Полторак Александр"
        >>> print MafiaGameRecord(5).exportToCsv()
        5,"2012-02-18",False,9995,9993,17,28,32,126,9996,56,9997,9998,28,9993,17,126,"Полторак Александр"
        >>> print MafiaGameRecord(1).exportToJson()
        {"don": 9993, "sheriff": 11, "players": [1, 57, 11, 81, 25, 63, 6, 9, 9993, 28], "wins": true, "mafs": [28, 9], "date": "2012-02-18", "gid": "1", "judge": "Полторак Александр"}
        """
        page = FetchStrategy.getGame(gid)
        ParseStrategy.parse(self, page)

    def exportToJson(self):
        """(MafiaGameRecord) -> str

        export game to json.
        """
        return json.dumps(self.__dict__, ensure_ascii=False)

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
