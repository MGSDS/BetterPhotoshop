#pragma once

#include "Core/Image/Image.hpp"

class Histogram
{
public:
    static std::vector<std::vector<int>> Calculate(const Image& image);
    static std::unique_ptr<Image> Correct(Image& image, float ignore = 0.0f);

    Histogram() = delete;
};
