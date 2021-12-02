import json
from collections import namedtuple

SAFE_AREA = 0.8

Point = namedtuple('Point', 'x y')


def same(a, b, precision=0.01):
    return abs(a - b) <= precision


def calculate_screen(_points, _width, _height, _safe_area):
    min_x = min(i[0] for i in _points)
    min_y = min(i[1] for i in _points)
    max_x = max(i[0] for i in _points)
    max_y = max(i[1] for i in _points)

    _x_center = (max_x + min_x) / 2
    _y_center = (max_y + min_y) / 2
    _x_scale = (max_x - min_x) / (_width * _safe_area)
    _y_scale = (max_y - min_y) / (_height * _safe_area)
    _screen_scale = max(_x_scale, _y_scale)
    return _x_center, _y_center, _screen_scale


Point = namedtuple('Point', 'x y')


class Screen:
    def __init__(self, width, height, x_center, y_center, screen_scale):
        self.width = width
        self.width_2 = width // 2
        self.height = height
        self.height_2 = height // 2
        self.x_center = x_center
        self.y_center = y_center
        self.screen_scale = screen_scale

    def to_screen_x(self, _x):
        return (_x - self.x_center) / self.screen_scale + self.width_2

    def to_screen_y(self, _y):
        return -(_y - self.y_center) / self.screen_scale + self.height_2

    def to_screen(self, _x, _y):
        return self.to_screen_x(_x), self.to_screen_y(_y)

    def from_screen_x(self, _x):
        return (_x - self.width_2) * self.screen_scale + self.x_center

    def from_screen_y(self, _y):
        return self.y_center - (_y - self.height_2) * self.screen_scale

    def from_screen(self, _x, _y):
        return self.from_screen_x(_x), self.from_screen_y(_y)

    def save(self, filename):
        data = {
            "width": self.width,
            "height": self.height,
            "x_center": self.x_center,
            "y_center": self.y_center,
            "screen_scale": self.screen_scale,
        }
        with open(filename, "w") as fp:
            json.dump(data, fp, indent=4)

    @staticmethod
    def load(filename):
        with open(filename, "r") as fp:
            data = json.load(fp)
            return Screen(**data)

    @property
    def min_x(self):
        return self.from_screen_x(0)

    @property
    def max_x(self):
        return self.from_screen_x(self.width-1)

    @property
    def max_y(self):
        return self.from_screen_y(0)

    @property
    def min_y(self):
        return self.from_screen_y(self.height-1)
