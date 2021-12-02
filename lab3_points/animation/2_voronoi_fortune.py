# from PIL import Image, ImageDraw
from math import sqrt, floor, ceil

from utils import *


if __name__ == '__main__':
    with open("points.json") as fp:
        points = json.load(fp)
    screen = Screen.load("screen.json")

    left = screen.from_screen_x(0)
    right = screen.from_screen_x(screen.width)
    top = screen.from_screen_y(0)
    bottom = screen.from_screen_y(screen.height)

    kadr = 0

    arcs = []
    events_queue = [
        {
            "type": "point",
            "pos": Point(p[0], p[1]),
            "color": p[2]
        } for p in sorted(points, key=lambda a: -a[1])
    ]

    up_y = int(ceil(events_queue[0]["pos"].y))
    last_point = events_queue[-1]["pos"]
    down_y = int(floor(bottom - sqrt((left - last_point.x)**2 + (bottom - last_point.y)**2)))

    kadr = 0

    for coast_y in range(up_y, down_y, -1):
        while events_queue and events_queue[0]["pos"].y > coast_y:
            event = events_queue.pop(0)
            

        kadr += 1

    # while points_queue or arc_queue:

