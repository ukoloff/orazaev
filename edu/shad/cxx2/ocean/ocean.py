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

        pygame.sprite.Sprite.__init__(self)
        self.size = size
        self.image = pygame.Surface(self.size, pygame.SRCALPHA)
        self.rect = pygame.Rect(position, size)

    def draw(self, surface):
        """(CommonSprite, pygame.Surface) -> NoneType"""

        surface.blit(self.image, self.rect)


class FishStrategy(object):
    """Fish strategy in ocean."""

    def __init__(self, fish):
        """(FishStrategy, fish) -> NoneType"""

        self.fish = fish
        self.quantumsPerTurn = self.fish.config['ocean']['quantums_per_turn']
        self.destination = self.fish.position
        self.speed = (0, 0)

    def _chooseDirection(self, directions):
        """(FishStrategy, list of (int, int)) -> (int, int)"""

        return directions[randint(0, len(directions) - 1)]

    def updateBetweenTurns(self, quantum):
        """(FishStrategy, int) -> NoneType"""

        assert quantum < self.quantumsPerTurn
        self.fish.rect.left = self.fish.position[0] * self.fish.size[0] \
                            + self.speed[0] * quantum
        self.fish.rect.top = self.fish.position[1] * self.fish.size[1] \
                            + self.speed[1] * quantum

    def updateMakeTurn(self, ocean):
        """(FishStrategy, dict, dict) -> NoneType"""

        self.fish.position = self.destination
        self._normalizeImage()

        if self.fish.starvation != 0:
            self.fish.starvation -= 1
        else:
            self.fish.die()
            del ocean.mapPositionToCreature[self.fish.position]
            return

        self._changeDestination(ocean)
        if self.speed == (0, 0):
            return

        del ocean.mapPositionToCreature[self.fish.position]
        if ocean.mapPositionToCreature.has_key(self.destination):
            victim = ocean.mapPositionToCreature[self.destination]
            victim.position = self.destination
            victim.strategy._normalizeImage()
            self.fish.kill(victim)

    def _changeDestination(self, ocean):
        """(FishStrategy, Ocean) -> NoneType"""

        directions = self._getPossibleDirections(ocean)

        if not directions:
            ocean.nextMap[self.fish.position] = self.fish
            self.speed = (0, 0)
            return

        direction = self._chooseDirection(directions)
        self.destination = (self.fish.position[0] + direction[0],
                    self.fish.position[1] + direction[1])
        ocean.nextMap[self.destination] = self.fish

        if direction[0] * self.fish.moveDirection < 0:
            self.fish.image = pygame.transform.flip(self.fish.image, True, False)
            self.fish.moveDirection = direction[0]

        self.speed = (direction[0] * self.fish.size[0] / float(self.quantumsPerTurn),
                      direction[1] * self.fish.size[1] / float(self.quantumsPerTurn))


    def _getPossibleDirections(self, ocean):
        """(FishStrategy, dict, dict) -> list of (int, int)"""

        directions = []
        for direction in itertools.product((1, 0, -1), (1, 0, -1)):
            newPosition = (self.fish.position[0] + direction[0],
                           self.fish.position[1] + direction[1])
            if ocean.mapPositionToCreature.has_key(newPosition) \
                or ocean.nextMap.has_key(newPosition) \
                or not (0 <= newPosition[0] < self.fish.config['ocean']['grid_size'][0]) \
                or not (0 <= newPosition[1] < self.fish.config['ocean']['grid_size'][1]):
                continue

            directions.append(direction)

        return directions

    def _normalizeImage(self):
        """(FishStrategy) -> NoneType"""

        self.fish.rect.left = self.fish.position[0] * self.fish.size[0]
        self.fish.rect.top = self.fish.position[1] * self.fish.size[1]



class DiedFishStrategy(FishStrategy):
    """What to do if you are died fish? :)"""

    def __init__(self, fish):
        """(DiedFishStrategy, fish) -> NoneType"""

        self.fish = fish
        self.quantumsPerTurn = self.fish.config['ocean']['quantums_per_turn']

    def updateBetweenTurns(self, quantum):
        """(DiedFishStrategy, fish) -> NoneType"""

        for key in self.fish.colors.keys():
            self.fish.colors[key] = (self.fish.colors[key][0],
                    self.fish.colors[key][1],
                    self.fish.colors[key][2],
                    255 * (self.quantumsPerTurn - quantum - 1) / self.quantumsPerTurn)
        self.fish._fillSprite()
        if self.fish.moveDirection < 0:
            self.fish.image = pygame.transform.flip(self.fish.image, True, False)

    def updateMakeTurn(self, ocean):
        """(DiedFishStrategy, dict, dict) -> NoneType"""

        pass



class PredatorStrategy(FishStrategy):
    """Predators strategy."""

    def _getPossibleDirections(self, ocean):
        """(FishStrategy, dict, dict) -> list of (int, int)"""

        directions = []
        foodNear = False
        for direction in itertools.product((1, 0, -1), (1, 0, -1)):
            newPosition = (self.fish.position[0] + direction[0],
                           self.fish.position[1] + direction[1])

            if not (0 <= newPosition[0] < ocean.gridSize[0]) \
                or not (0 <= newPosition[1] < ocean.gridSize[1]) \
                or direction == (0, 0):
                continue

            if ocean.mapPositionToCreature.has_key(newPosition) \
                and ocean.mapPositionToCreature[newPosition].eatable \
                and ocean.mapPositionToCreature[newPosition].alive:
                if foodNear:
                    directions = [direction]
                else:
                    directions.append(direction)
                    foodNear = True
                continue

            if foodNear or ocean.nextMap.has_key(newPosition):
                continue

            directions.append(direction)

        return directions



class Fish(CommonSprite):
    """Fish in ocean."""

    def __init__(self, position, size, config):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        CommonSprite.__init__(self, (position[0] * size[0],
                position[1] * size[1]), size)

        self.alive = True
        self.position = position
        self.config = config
        self.moveDirection = 1
        self.strategy = FishStrategy(self)

        self._fillSprite()

    def updateTurn(self, ocean):
        """(Fish, dict, dict) -> NoneType"""

        self.strategy.updateMakeTurn(ocean)

    def update(self, quantum):
        """(Fish, int) -> NoneType"""

        self.strategy.updateBetweenTurns(quantum)

    def die(self):
        """(Fish) -> NoneType"""

        self.alive = False
        self.strategy = DiedFishStrategy(self)

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

        if self.alive:
            pygame.draw.rect(self.image, self.colors['eye'], pygame.Rect(7 * zoom[0], 2 * zoom[1], 1 * zoom[0], 1 * zoom[1]))
        else:
            pygame.draw.line(self.image, self.colors['eye'], (7 * zoom[0], 2 * zoom[1]), (8 * zoom[0], 3 * zoom[1]), 3)
            pygame.draw.line(self.image, self.colors['eye'], (8 * zoom[0], 2 * zoom[1]), (7 * zoom[0], 3 * zoom[1]), 3)



class Victim(Fish):
    """Victim in ocean."""

    def __init__(self, position, size, config):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        self.colors = config['victim']['colors']
        Fish.__init__(self, position, size, config)
        self.starvation = -1
        self.eatable = True


class Predator(Fish):
    """Predator in ocean."""

    def __init__(self, position, size, config):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        self.colors = config['predator']['colors']
        Fish.__init__(self, position, size, config)
        self.strategy = PredatorStrategy(self)
        self.starvation = config['predator']['starvation']
        self.eatable = False

    def kill(self, other):
        """(Fish, Fish) -> NoneType"""

        other.die()
        self.starvation = self.config['predator']['starvation']


class Ocean(CommonSprite):
    """Ocean with victims and predators."""

    def __init__(self, config):
        """(Ocean, dict) -> NoneType"""

        CommonSprite.__init__(self, (0, 0), config['ocean']['size'])
        self.config = config

        self.numVictims = config['ocean']['num_victims']
        self.numPredators = config['ocean']['num_predators']
        self.quantumsPerTurn = config['ocean']['quantums_per_turn']

        self._createGrid()
        self._fillOcean()

        self.currentQuantum = self.quantumsPerTurn
        self.nextMap = self.mapPositionToCreature

        self._updateImage()

    def _createGrid(self):
        """(Ocean) -> NoneType"""

        self.gridSize = self.config['ocean']['grid_size']
        self.zoomFactor = (self.image.get_size()[0] / float(self.gridSize[0]),
                          self.image.get_size()[1] / float(self.gridSize[1]))

    def _drawGrid(self):
        """(Ocean) -> NoneType"""

        for x in xrange(self.gridSize[0]):
            pygame.draw.line(self.image, self.config['ocean']['grid_color'], (x * self.zoomFactor[0], 0), (x * self.zoomFactor[0], self.size[1]), 1)

        for y in xrange(self.gridSize[1]):
            pygame.draw.line(self.image, self.config['ocean']['grid_color'], (0, y * self.zoomFactor[1]), (self.size[0], y * self.zoomFactor[1]), 1)

    def _getFreePosition(self):
        """(Ocean) -> NoneType"""

        while True:
            position = (randint(0, self.gridSize[0] -1),
                        randint(0, self.gridSize[1] - 1))
            try:
                self.mapPositionToCreature[position]
            except (KeyError):
                return position

    def _updateImage(self):
        """(Ocean) -> NoneType"""

        self.image.fill(self.config['ocean']['color'])
        self._drawGrid()
        for creature in self.creatures:
            creature.draw(self.image)

    def update(self):
        """(Ocean) -> NoneType"""

        if self.currentQuantum == self.quantumsPerTurn:
            self._updateCreaturesDestinations()
            self.currentQuantum = 0
        else:
            self._moveCreatures()

        self.currentQuantum += 1
        self._updateImage()

    def _moveCreatures(self):
        """(Ocean) -> NoneType"""

        for creature in self.creatures:
            creature.update(self.currentQuantum)

    def _updateCreaturesDestinations(self):
        """(Ocean) -> NoneType"""

        self.mapPositionToCreature = self.nextMap
        self.nextMap = {}
        creatures = self.mapPositionToCreature.items()
        self.creatures = self.mapPositionToCreature.values()
        # Shuffle creatures here

        for position, creature in creatures:
            creature.updateTurn(self)
            # del self.mapPositionToCreature[position]
        #self.mapPositionToCreature = self.nextMap


    def _fillOcean(self):
        """(Ocean) -> NoneType"""

        self.mapPositionToCreature = {}
        self.creatures = []
        if self.numVictims + self.numPredators > prod(self.gridSize):
            raise Exception("Can't create ocean becouse number of creatures is greater than ocean.")

        for x in xrange(self.numVictims):
            self._addVictim(self._getFreePosition())

        for x in xrange(self.numPredators):
            self._addPredator(self._getFreePosition())

    def _addCreature(self, creature):
        """(Ocean, Fish) -> NoneType"""

        try:
            self.mapPositionToCreature[creature.position]
        except (KeyError):
            self.mapPositionToCreature[creature.position] = creature
            self.creatures.append(creature)
        else:
            raise Exception("There are another creature in position!")

    def _addPredator(self, position):
        """(Ocean, (int, int)) -> NoneType"""

        self._addCreature(Predator(position, self.zoomFactor, self.config))

    def _addVictim(self, position):
        """(Ocean, (int, int)) -> NoneType"""

        self._addCreature(Victim(position, self.zoomFactor, self.config))



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
        pygame.time.delay(config['ocean']['quantum'])
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
