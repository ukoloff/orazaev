#!/usr/bin/env python

import pygame
from pygame.locals import *

import sys

width = 550
height = 480
oceanColor = (0, 7, 160)

class Fish(pygame.sprite.Sprite):
    def __init__(self, position, size):
        """(Fish, (int, int), (int, int) -> NoneType"""
        self.rect = pygame.Rect(position, size)

    def draw(self):
        pass

class Victim(Fish):
    def __init__(self, position, size, config):
        """(Fish, (int, int), (int, int) -> NoneType"""
        # Call create sprite here
        pass

class Predator(Fish):
    def __init__(self, position, size, config):
        """(Fish, (int, int), (int, int) -> NoneType"""
        # Call create sprite here
        pass

class Ocean(object):
    def __init__(self, config):
        """(Ocean, dict) -> NoneType"""
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
    fish = CreateFishSprite((9, 6), config)
    while True:
        fish.rect.move_ip(speed)

        if fish.rect.left < 0 or fish.rect.right > width:
            speed[0] = -speed[0]
            fish.image = pygame.transform.flip(fish.image, True, False)
        if fish.rect.top < 0 or fish.rect.bottom > height:
            speed[1] = -speed[1]

        window.fill(oceanColor)
        #fish.draw(window)
        window.blit(fish.image, fish.rect)
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

def CreateFishSprite(size, config):
    finsColor = config['victim']['colors']['fins']
    gillsColor = config['victim']['colors']['gills']
    bodyColor = config['victim']['colors']['body']
    headColor = config['victim']['colors']['head']
    eyeColor = config['victim']['colors']['eye']

    sprite = pygame.sprite.Sprite()
    sprite.image = pygame.Surface(size)
    sprite.image.fill(config['ocean']['color'])

    zoom = (size[0] / 9, size[1] / 6)

    # Draw head
    pygame.draw.rect(sprite.image, headColor, pygame.Rect(5 * zoom[0], 1 * zoom[1], 3 * zoom[0], 4 * zoom[1]))
    pygame.draw.rect(sprite.image, headColor, pygame.Rect(7 * zoom[0], 2 * zoom[1], 2 * zoom[0], 3 * zoom[1]))

    # Draw body
    pygame.draw.rect(sprite.image, bodyColor, pygame.Rect(3 * zoom[0], 1 * zoom[1], 3 * zoom[0], 4 * zoom[1]))
    pygame.draw.rect(sprite.image, bodyColor, pygame.Rect(2 * zoom[0], 2 * zoom[1], 2 * zoom[0], 2 * zoom[1]))

    # Draw fins
    pygame.draw.rect(sprite.image, finsColor, pygame.Rect(0 * zoom[0], 1 * zoom[1], 2 * zoom[0], 4 * zoom[1]))
    pygame.draw.rect(sprite.image, finsColor, pygame.Rect(4 * zoom[0], 0 * zoom[1], 2 * zoom[0], 1 * zoom[1]))
    pygame.draw.rect(sprite.image, finsColor, pygame.Rect(4 * zoom[0], 5 * zoom[1], 2 * zoom[0], 1 * zoom[1]))

    # Draw gills
    pygame.draw.rect(sprite.image, gillsColor, pygame.Rect(6 * zoom[0], 1 * zoom[1], 1 * zoom[0], 1 * zoom[1]))
    pygame.draw.rect(sprite.image, gillsColor, pygame.Rect(5 * zoom[0], 2 * zoom[1], 1 * zoom[0], 2 * zoom[1]))
    pygame.draw.rect(sprite.image, gillsColor, pygame.Rect(6 * zoom[0], 4 * zoom[1], 1 * zoom[0], 1 * zoom[1]))

    # Draw eye
    pygame.draw.rect(sprite.image, eyeColor, pygame.Rect(7 * zoom[0], 2 * zoom[1], 1 * zoom[0], 1 * zoom[1]))

    sprite.rect = sprite.image.get_rect()

    return sprite


if __name__ == '__main__':
    main()
