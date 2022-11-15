#pragma once

#include "Core/Image/Image.hpp"
#include <cmath>

class Painter
{
public:
    static Image DrawLine(
            const Image& image,
            std::pair<float, float> a,
            std::pair<float, float> b,
            int width,
            float imageGamma,
            ActiveChannel channel,
            Pixel color);
private:
    Painter() { }
    static Image DrawLine(int im_width, int im_height, float x0, float y0, float x1, float y1, int width, Pixel color);
    static void SetPixel(Image& image, int x, int y, Pixel color, float alpha);
};
