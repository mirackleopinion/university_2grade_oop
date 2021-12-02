from PIL import Image, ImageDraw

from utils import *


def point_and_line(_a, _b, _c, _x, _y):
    return _a * _x + _b * _y + _c


if __name__ == '__main__':
    with open("points.json") as fp:
        points = json.load(fp)
    screen = Screen.load("screen.json")

    cells = []
    kadr = 0
    # перебираем все точки, ищем для каждой цепочку координат выпуклого многоугольника диаграммы Вороного
    for i, (x1, y1, cell_color) in enumerate(points):
        # начальный многоугольник - границы экрана
        chain = [
            Point(screen.from_screen_x(0), screen.from_screen_y(0)),
            Point(screen.from_screen_x(screen.width), screen.from_screen_y(0)),
            Point(screen.from_screen_x(screen.width), screen.from_screen_y(screen.height)),
            Point(screen.from_screen_x(0), screen.from_screen_y(screen.height))
        ]
        # отсекаем многоугольник серединными перпендикулярами к другим точкам
        for j, (x2, y2, _) in enumerate(points):
            # отбрасываем проверку с собой
            if i == j:
                continue

            # (xm, ym) - середина отрезка ((x1, y1), (x2, y2))
            # xm = (x1+x2)/2
            # ym = (y1+y2)/2
            # Ax + By + C = 0 это уравнение прямой для точек (x1, y1) и (x2, y2)
            # A = y1 - y2
            # B = x2 - x1
            # уравнение серединного перпендикуляра:
            # A(y - ym) - B(x - xm) = 0
            # (x1 - x2)*x + (y1 - y2)*y + ()/2 = 0
            # A и B не могут быть одновременно равны нулю
            # если в уравнение прямой серединного перпендикуляра подставить точку:
            # (x1, y1), ответ > 0
            # (x2, y2), ответ < 0
            # (xm, xm), ответ = 0

            a = x1 - x2
            b = y1 - y2
            c = (x2**2 - x1**2 + y2**2 - y1**2) / 2.0

            if same(a, 0) and same(b, 0):
                raise ValueError(f"points {i} and {j} too close")

            chain_pos = [point_and_line(a, b, c, joint.x, joint.y) for joint in chain]

            # весь серединный перпендикуляр вне выпуклого многоугольника
            if all(pos >= 0 for pos in chain_pos):
                continue

            vertex_count = len(chain)
            edges = [
                [
                    chain[vertex_number],
                    chain[(vertex_number + 1) % vertex_count],
                    chain_pos[vertex_number],
                    chain_pos[(vertex_number + 1) % vertex_count]
                ] for vertex_number in range(vertex_count)
                if chain_pos[vertex_number] >= 0 or chain_pos[(vertex_number + 1) % vertex_count] >= 0
            ]

            edges_to_cut = []
            for edge_num, [point1, point2, score1, score2] in enumerate(edges):
                if score1 < 0 <= score2:
                    second_edge_to_cut = edge_num
                if score1 >= 0 > score2:
                    first_edge_to_cut = edge_num

            # fix first edge
            point1, point2, score1, score2 = edges[first_edge_to_cut]
            cutter = score2 / (score2 - score1)
            new_point1 = Point((point1.x - point2.x) * cutter + point2.x, (point1.y - point2.y) * cutter + point2.y)
            edges[first_edge_to_cut][1 if score2 < 0 else 0] = new_point1

            # fix second edge
            point1, point2, score1, score2 = edges[second_edge_to_cut]
            cutter = score2 / (score2 - score1)
            new_point2 = Point((point1.x - point2.x) * cutter + point2.x, (point1.y - point2.y) * cutter + point2.y)
            edges[second_edge_to_cut][1 if score2 < 0 else 0] = new_point2

            # insert new edge after first but before second edge
            edges.insert(second_edge_to_cut, [new_point1, new_point2, 0.0, 0.0])

            # update convex chain
            chain = [p1 for p1, p2, s1, s2 in edges if not (same(p1.x, p2.x) and same(p1.y, p2.y))]

            # animate
            img = Image.new('RGB', (screen.width, screen.height), color="white")
            draw = ImageDraw.Draw(img)
            draw.rectangle((0, 0, screen.width - 1, screen.height - 1), fill="white")
            radius = 2

            for segment, segment_color, center in cells:
                draw.polygon(xy=[screen.to_screen(el.x, el.y) for el in segment], fill=segment_color, outline="black")

            draw.polygon(xy=[screen.to_screen(el.x, el.y) for el in chain], fill=cell_color, outline="black")

            for segment, segment_color, center in cells:
                pxw, pyw = screen.to_screen(center.x, center.y)
                draw.ellipse((pxw - radius, pyw - radius, pxw + radius, pyw + radius), fill="white", outline="black")

            img.save(f"naive/{kadr:05}.png")
            kadr += 1

        cells.append((chain, cell_color, Point(x1, y1)))

    with open('naive_voronoi.json', 'w') as f:
        json.dump(cells, f, indent=4)
    # save image
    img = Image.new('RGB', (screen.width, screen.height), color="white")
    draw = ImageDraw.Draw(img)
    draw.rectangle((0, 0, screen.width - 1, screen.height - 1), fill="white")
    radius = 2

    for segment, segment_color, center in cells:
        draw.polygon(xy=[screen.to_screen(el.x, el.y) for el in segment], fill=segment_color, outline="black")
        pxw, pyw = screen.to_screen(center.x, center.y)
        draw.ellipse((pxw - radius, pyw - radius, pxw + radius, pyw + radius), fill="white", outline="black")

    for segment, segment_color, center in cells:
        pxw, pyw = screen.to_screen(center.x, center.y)
        draw.text(xy=(pxw, pyw + radius), fill="black", text=f'({center.x}, {center.y})', align="center")

    img.save(f"naive.png")
