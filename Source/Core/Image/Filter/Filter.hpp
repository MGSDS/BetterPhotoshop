#pragma once

#include "Core/Image/Image.hpp"

class Filter
{
public:
    virtual std::unique_ptr<Image> Apply(const Image& image) = 0;
    virtual ~Filter() = default;
};