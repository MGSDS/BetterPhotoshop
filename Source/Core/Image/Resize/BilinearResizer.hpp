#pragma once

#include "Resizer.hpp"

class BilinearResizer : public Resizer
{
public:
    std::unique_ptr<Image> Apply(const Image& img, float scaleX, float scaleY) override;
};