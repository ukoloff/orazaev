#!/usr/bin/env python

import pygame
from pygame.locals import *

import sys

oceanColor = (0, 7, 160)

class Fish(pygame.sprite.Sprite):
    "Fish in ocean."
    def __init__(self, position, size, colors):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        self.size = size
        self.colors = colors
        self.rect = pygame.Rect(position, size)
        self.image = pygame.Surface(self.size, pygame.SRCALPHA)
        self.moveDirection = 1

        self._fillSprite()

    def draw(self, surface):
        """(Fish, pygame.Surface) -> NoneType"""

        surface.blit(self.image, self.rect)

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

class Predator(Fish):
    """Predator in ocean."""
    def __init__(self, position, size, config):
        """(Fish, (int, int), (int, int), dict) -> NoneType"""

        Fish.__init__(sefl, position, size, colors)

class Ocean(object):
    def __init__(self, config):
        """(Ocean, dict) -> NoneType"""

        self.config = config
        self.surface = pygame.Surface(config['ocean']['size'])
        self.surface.fill(config['ocean']['color'])

        self.predators = {}
        self.victioms = {}

    def _fillOcean_(self):
        """(Ocean) -> NoneType"""
        for x in xrange(config['ocean']['num_victims']):
            pass

        for x in xrange(config['ocean']['num_predators']):
            pass

    def _addPredator_(self):
        """(Ocean) -> NoneType"""
        pass

    def _addVictim_(self):
        """(Ocean) -> NoneType"""
        pass

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
    speed = [2, 2]
    fishSize = (9 * 10, 6 * 10)

    from random import randint
    victims = []
    for x in xrange(randint(3, 7)):
        position = (randint(0, config['ocean']['size'][0] - fishSize[0] - 1),
                   randint(0, config['ocean']['size'][1] - fishSize[1] - 1))
        fish = Fish(position, fishSize, config['victim']['colors'])
        victims.append((fish, [2 - randint(0, 1) * 4, 2 - randint(0, 1) * 4]))

    predators = []
    for x in xrange(randint(3, 7)):
        position = (randint(0, config['ocean']['size'][0] - fishSize[0] - 1),
                   randint(0, config['ocean']['size'][1] - fishSize[1] - 1))
        fish = Fish(position, fishSize, config['predator']['colors'])
        victims.append((fish, [2 - randint(0, 1) * 4, 2 - randint(0, 1) * 4]))

    while True:
        window.fill(config['ocean']['color'])
        for fish in victims:
            fish[0].update(fish[1])
            if fish[0].rect.left < 0 or fish[0].rect.right > config['ocean']['size'][0]:
                fish[1][0] = -fish[1][0]
            if fish[0].rect.top < 0 or fish[0].rect.bottom > config['ocean']['size'][1]:
                fish[1][1] = -fish[1][1]

            fish[0].draw(window)

        for fish in predators:
            fish[0].update(fish[1])
            if fish[0].rect.left < 0 or fish[0].rect.right > config['ocean']['size'][0]:
                fish[1][0] = -fish[1][0]
            if fish[0].rect.top < 0 or fish[0].rect.bottom > config['ocean']['size'][1]:
                fish[1][1] = -fish[1][1]

            fish[0].draw(window)
        pygame.display.flip()

        pygame.time.delay(20)
        input(pygame.event.get())

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
