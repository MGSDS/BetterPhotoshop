#pragma once

#include "Resizer.hpp"

class NearestPointResizer : public Resizer
{
public:
    std::unique_ptr<Image> Apply(const Image& img, float scaleX, float scaleY) override;
};

