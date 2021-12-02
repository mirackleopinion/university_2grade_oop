# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
from random import randint

from PIL import ImageColor

from utils import *

if __name__ == '__main__':
    WIDTH = 1024
    HEIGHT = 768
    POINTS = 50

    points = [(randint(0, WIDTH), randint(0, HEIGHT), list(ImageColor.colormap.keys())[i]) for i in range(POINTS)]
    with open("points.json", "w") as fp:
        json.dump(points, fp, indent=4)

    x_center, y_center, screen_scale = calculate_screen(points, WIDTH, HEIGHT, SAFE_AREA)
    screen = Screen(WIDTH, HEIGHT, x_center, y_center, screen_scale)
    screen.save("screen.json")

