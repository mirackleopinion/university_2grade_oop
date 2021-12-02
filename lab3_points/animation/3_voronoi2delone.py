from PIL import Image, ImageDraw

from utils import *

if __name__ == '__main__':
    with open("points.json") as fp:
        points = json.load(fp)
    screen = Screen.load("screen.json")

    voronoi_cells = json.load(open('naive_voronoi.json'))
    delone_edges = []

    for i in range(len(voronoi_cells)):
        chain1, color1, center1 = voronoi_cells[i]
        for j in range(i + 1, len(voronoi_cells)):
            chain2, color2, center2 = voronoi_cells[j]
            found = False

            for point1 in chain1:
                if found:
                    continue
                for point2 in chain2:
                    if not found and same(point1[0], point2[0]) and same(point1[1], point2[1]):
                        found = True

            if found:
                delone_edges.append((center1, center2))

    img = Image.new('RGB', (screen.width, screen.height), color="white")
    draw = ImageDraw.Draw(img)
    draw.rectangle((0, 0, screen.width - 1, screen.height - 1), fill="white")
    radius = 2

    for segment, segment_color, center in voronoi_cells:
        draw.polygon(xy=[screen.to_screen(el[0], el[1]) for el in segment], fill=segment_color, outline="black")
        pxw, pyw = screen.to_screen(center[0], center[1])
        draw.ellipse((pxw - radius, pyw - radius, pxw + radius, pyw + radius), fill="white", outline="black")

    for center1, center2 in delone_edges:
        draw.line((screen.to_screen(*center1), screen.to_screen(*center2)), fill="red", width=3)

    img.save(f"voronoi2delone.png")

