from math import sin
from random import randint, choice
from collections import namedtuple

from PIL import Image, ImageDraw, ImageColor
WIDTH = 1920
HEIGHT = 1080

WIDTH_2 = WIDTH//2
HEIGHT_2 = HEIGHT//2

POINTS_COUNT = 4
Coords = namedtuple('Coords', 'x y color')

points = [Coords(randint(0, WIDTH), randint(0, HEIGHT), choice(list(ImageColor.colormap.keys())))
          for i in range(POINTS_COUNT)]

# points = []
# for x in range(1, 3):
#     for y in range(1, 3):
#         points.append(Coords(x, y, choice(list(ImageColor.colormap.keys()))))

min_x = points[0].x
max_x = points[0].x
min_y = points[0].y
max_y = points[0].y

for p in points[1:]:
    max_x = max(max_x, p.x)
    max_y = max(max_y, p.y)
    min_x = min(min_x, p.x)
    min_y = min(min_y, p.y)

x_center = (min_x + max_x) / 2
y_center = (min_y + max_y) / 2

x_scale = (max_x - min_x) / WIDTH
y_scale = (max_y - min_y) / HEIGHT
scale = max(x_scale, y_scale) / 0.9


def to_screen(_x, _y):
    return (_x - x_center)/scale + WIDTH_2, -(_y - y_center)/scale + HEIGHT_2


def to_world(_x, _y):
    return ((_x - WIDTH_2) * scale + x_center,
            y_center - (_y - HEIGHT_2) * scale)



img = Image.new("RGB", size=(WIDTH, HEIGHT), color=ImageColor.getrgb("white"))

draw = ImageDraw.Draw(img)

for x in range(0, WIDTH, 2):
    print(f'{x}')
    for y in range(0, HEIGHT, 2):
        xw, yw = to_world(x, y)
        dists = list((((p.x - xw)**2 + (p.y - yw)**2), p.color) for p in points)
        sorted_dists = list(sorted(dists, key=lambda x: x[0]))
        draw.point((x, y), fill=sorted_dists[0][1])


for x, y, color in points:
    x, y = to_screen(x, y)
    x, y = to_world(x, y)
    x, y = to_screen(x, y)
    draw.ellipse([(x - 3, y - 3), (x + 3, y + 3)], fill=color, outline='black')
    draw.ellipse([(x - 2, y - 2), (x + 2, y + 2)], fill=color, outline='white')

img.show()
img.save('1.png')