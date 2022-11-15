#include "Painter.hpp"

Image Painter::DrawLine(const Image& image, std::pair<float, float> a, std::pair<float, float> b, int width) {
    //Bresenham's line algorithm
    auto res = Image(image);
    int x0 = static_cast<int>(a.first);
    int y0 = static_cast<int>(a.second);
    int x1 = static_cast<int>(b.first);
    int y1 = static_cast<int>(b.second);

    DrawLine(res, x0, y0, x1, y1);
    y0 -= width / 2;
    y1 -= width / 2;
    while (--width > 0) {
        DrawLine(res, x0, y0, x1, y1);
        y0++;
        y1++;
    }
    return res;
}

void Painter::DrawLine(Image &image, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    SetPixel(image, x1, y1);
    while (x0 != x1 || y0 != y1) {
        SetPixel(image, x0, y0);
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Painter::SetPixel(Image &image, int x, int y, float c0, float c1, float c2, float c3) {
    if (x < 0 || x >= image.GetWidth() || y < 0 || y >= image.GetHeight()) {
        return;
    }
    image.PixelAt(y, x) = Pixel(c0, c1, c2, c3);
}
