#pragma once

#include "Core/Image/Image.hpp"

#include <cmath>

class Painter
{
public:
    Painter() = delete;

    static Image DrawLine(size_t imWidth, size_t imHeight, float x0, float y0, float x1, float y1, int width, const Pixel& color);

private:
    static void SetPixel(Image& image, int x, int y, Pixel color, float alpha);
};
