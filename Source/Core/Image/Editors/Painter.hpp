#pragma once

#include "Core/Image/Image.hpp"

class Painter {
public:
    static Image DrawLine(const Image& image, float gama, std::pair<float, float> a, std::pair<float, float> b, int width);
private:
    Painter() { }

};
