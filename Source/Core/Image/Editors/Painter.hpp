#pragma once

#include "Core/Image/Image.hpp"

#include <cmath>

class Painter
{
public:
    static Image DrawLine(size_t im_width, size_t im_height, float x0, float y0, float x1, float y1, int width, Pixel color);

private:
    Painter() {}

    static void SetPixel(Image& image, int x, int y, Pixel color, float alpha);
};
