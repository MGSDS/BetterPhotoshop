#pragma once

#include "Core/Image/Image.hpp"

class Histogram
{
public:
    static std::vector<std::vector<int>> Calculate(const Image& image);
    static std::unique_ptr<Image> Correct(Image& image, const std::vector<std::vector<int>>& histogram);

    Histogram() = delete;
};
