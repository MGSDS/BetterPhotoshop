#pragma once

#include "Core/Image/Image.hpp"

class Painter {
public:
    static Image DrawLine(const Image& image, std::pair<float, float> a, std::pair<float, float> b, int width);
private:
    Painter() { }
    static void DrawLine(Image& image, int x0, int y0, int x1, int y1);
    static void SetPixel(Image& image, int x, int y, float c0 = 0.0f, float c1 = 0.0f, float c2 = 0.0f, float c3 = 1.0f);
};
