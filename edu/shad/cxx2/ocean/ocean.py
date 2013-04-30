#!/usr/bin/env python

import pygame
import itertools
from pygame.locals import *
from operator import mul
from random import randint

import sys

def prod(iterable):
    """prod(list or tuple of ints or floats) -> int or float

    >>> prod((1, 2, 3))
    6
    """

    return reduce(mul, iterable, 1)

class CommonSprite(pygame.sprite.Sprite):
    """CommonSprite -- common sprite for ocean app."""

    def __init__(self, position, size):
        """(CommonSprite, (int, int), (int, int)) -> NoneType"""

        self.size = size
        self.image = pygame.Surface(self.size, pygame.SRCALPHA)
        self.rect = pygame.Rect(position, size)

    def draw(self, surface):
        """(CommonSprite, pygame.Surface) -> NoneType"""

        surface.blit(self.image, self.rect)

class Fish(CommonSprite):
    """Fish in ocean."""

    def __init__(self, position, size, colors):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        CommonSprite.__init__(self, position, size)

        self.colors = colors
        self.moveDirection = 1

        self._fillSprite()

    def update(self, direction):
        """(Fish, (int, int)) -> NoneType"""

        self.rect.move_ip(direction)
        if direction[0] * self.moveDirection < 0:
            self.image = pygame.transform.flip(self.image, True, False)
            self.moveDirection = direction[0]

    def _fillSprite(self):
        """(Fish) -> NoneType"""

        self.image.fill((0, 0, 0, 0))
        zoom = (self.size[0] / 9, self.size[1] / 6)

        # Draw head
        pygame.draw.rect(self.image, self.colors['head'], pygame.Rect(5 * zoom[0], 1 * zoom[1], 3 * zoom[0], 4 * zoom[1]))
        pygame.draw.rect(self.image, self.colors['head'], pygame.Rect(7 * zoom[0], 2 * zoom[1], 2 * zoom[0], 3 * zoom[1]))

        # Draw body
        pygame.draw.rect(self.image, self.colors['body'], pygame.Rect(3 * zoom[0], 1 * zoom[1], 3 * zoom[0], 4 * zoom[1]))
        pygame.draw.rect(self.image, self.colors['body'], pygame.Rect(2 * zoom[0], 2 * zoom[1], 2 * zoom[0], 2 * zoom[1]))

        # Draw fins
        pygame.draw.rect(self.image, self.colors['fins'], pygame.Rect(0 * zoom[0], 1 * zoom[1], 2 * zoom[0], 4 * zoom[1]))
        pygame.draw.rect(self.image, self.colors['fins'], pygame.Rect(4 * zoom[0], 0 * zoom[1], 2 * zoom[0], 1 * zoom[1]))
        pygame.draw.rect(self.image, self.colors['fins'], pygame.Rect(4 * zoom[0], 5 * zoom[1], 2 * zoom[0], 1 * zoom[1]))

        # Draw gills
        pygame.draw.rect(self.image, self.colors['gills'], pygame.Rect(6 * zoom[0], 1 * zoom[1], 1 * zoom[0], 1 * zoom[1]))
        pygame.draw.rect(self.image, self.colors['gills'], pygame.Rect(5 * zoom[0], 2 * zoom[1], 1 * zoom[0], 2 * zoom[1]))
        pygame.draw.rect(self.image, self.colors['gills'], pygame.Rect(6 * zoom[0], 4 * zoom[1], 1 * zoom[0], 1 * zoom[1]))

        # Draw eye
        pygame.draw.rect(self.image, self.colors['eye'], pygame.Rect(7 * zoom[0], 2 * zoom[1], 1 * zoom[0], 1 * zoom[1]))

class Victim(Fish):
    """Victim in ocean."""

    def __init__(self, position, size, colors):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        Fish.__init__(self, position, size, colors)

    def update(self, position, directions, newMap):
        """(Victim, list of (int, int)) -> NoneType"""

        if not directions:
            newMap[position] = self
            return

        n = randint(0, len(directions) - 1)
        position = (position[0] + directions[n][0],
                    position[1] + directions[n][1])
        newMap[position] = self
        Fish.update(self, directions[n])
        self.rect.left = position[0] * self.size[0]
        self.rect.top = position[1] * self.size[1]

class Predator(Fish):
    """Predator in ocean."""

    def __init__(self, position, size, colors):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        Fish.__init__(self, position, size, colors)

    def update(self, position, directions, newMap):
        """(Predator, list of (int, int)) -> NoneType"""

        if not directions:
            newMap[position] = self
            return

        n = randint(0, len(directions) - 1)
        position = (position[0] + directions[n][0],
                    position[1] + directions[n][1])
        newMap[position] = self
        Fish.update(self, directions[n])
        self.rect.left = position[0] * self.size[0]
        self.rect.top = position[1] * self.size[1]

class Ocean(CommonSprite):
    """Ocean with victims and predators."""
    def __init__(self, config):
        """(Ocean, dict) -> NoneType"""

        CommonSprite.__init__(self, (0, 0), config['ocean']['size'])
        self.config = config

        self.numVictims = config['ocean']['num_victims']
        self.numPredators = config['ocean']['num_predators']

        self._createGrid_()
        self._fillOcean_()
        self._updateImage_()

    def _createGrid_(self):
        """(Ocean) -> NoneType"""

        self.gridSize = self.config['ocean']['grid_size']
        self.zoomFactor = (self.image.get_size()[0] / float(self.gridSize[0]),
                          self.image.get_size()[1] / float(self.gridSize[1]))

    def draw(self, surface):
        """(CommonSprite, pygame.Surface) -> NoneType"""

        surface.blit(self.image, self.rect)

    def _drawGrid_(self):
        """(Ocean) -> NoneType"""

        for x in xrange(self.gridSize[0]):
            pygame.draw.line(self.image, self.config['ocean']['grid_color'], (x * self.zoomFactor[0], 0), (x * self.zoomFactor[0], self.size[1]), 1)

        for y in xrange(self.gridSize[0]):
            pygame.draw.line(self.image, self.config['ocean']['grid_color'], (0, y * self.zoomFactor[1]), (self.size[0], y * self.zoomFactor[1]), 1)

    def _getFreePosition_(self):
        """(Ocean) -> NoneType"""

        while True:
            position = (randint(0, self.gridSize[0] -1),
                        randint(0, self.gridSize[1] - 1))
            try:
                self.mapPositionToCreature[position]
            except (KeyError):
                return position

    def _updateImage_(self):
        """(Ocean) -> NoneType"""

        self.image.fill(self.config['ocean']['color'])
        self._drawGrid_()
        for creature in self.mapPositionToCreature.values():
            creature.draw(self.image)

    def update(self):
        """(Ocean) -> NoneType"""

        newMap = {}
        creatures = self.mapPositionToCreature.items()

        for position, creature in creatures:
            directions = []
            for direction in itertools.product((1, 0, -1), (1, 0, -1)):
                newPosition = (position[0] + direction[0], position[1] + direction[1])
                if self.mapPositionToCreature.has_key(newPosition) \
                    or newMap.has_key(newPosition) \
                    or not (0 <= newPosition[0] < self.gridSize[0]) \
                    or not (0 <= newPosition[1] < self.gridSize[1]):
                    continue

                directions.append(direction)
            creature.update(position, directions, newMap)
            del self.mapPositionToCreature[position]
        self.mapPositionToCreature = newMap

        self._updateImage_()

    def _fillOcean_(self):
        """(Ocean) -> NoneType"""

        self.mapPositionToCreature = {}
        if self.numVictims + self.numPredators > prod(self.gridSize):
            raise Exception("Can't create ocean becouse number of creatures is greater than ocean.")

        for x in xrange(self.numVictims):
            self._addVictim_(self._getFreePosition_())

        for x in xrange(self.numPredators):
            self._addPredator_(self._getFreePosition_())

    def _addCreature_(self, creature, position):
        """(Ocean, Fish, (int, int)) -> NoneType"""

        try:
            self.mapPositionToCreature[position]
        except (KeyError):
            self.mapPositionToCreature[position] = creature
        else:
            raise Exception("There are another creature in position!")

    def _addPredator_(self, position):
        """(Ocean) -> NoneType"""

        self._addCreature_(Predator((position[0] * self.zoomFactor[0],
                    position[1] * self.zoomFactor[1]),
                    self.zoomFactor, self.config['predator']['colors']),
                    position)

    def _addVictim_(self, position):
        """(Ocean) -> NoneType"""

        self._addCreature_(Victim((position[0] * self.zoomFactor[0],
                    position[1] * self.zoomFactor[1]),
                    self.zoomFactor, self.config['victim']['colors']),
                    position)

def init_window(config):
    pygame.init()
    window = pygame.display.set_mode(config['window']['size'])
    pygame.display.set_caption('Ocean')

    return window

def input(events):
    for event in events:
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            sys.exit(0)

def action(window, config):
    ocean = Ocean(config)
    while True:
        ocean.draw(window)
        pygame.display.flip()
        pygame.time.delay(200)
        input(pygame.event.get())
        ocean.update()

def loadConfig(stream):
    """(str) -> dict"""

    from yaml import load
    return load(stream)

def main():
    config = loadConfig(open('ocean.yaml', 'rb'))
    window = init_window(config)
    action(window, config)

if __name__ == '__main__':
    main()
