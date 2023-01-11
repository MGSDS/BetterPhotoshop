#pragma once

#include "Resizer.hpp"

class Lanczos3Resizer : public Resizer
{
public:
    std::unique_ptr<Image> Apply(const Image& img, float scaleX, float scaleY) override;

private:
    static double LanczosKernel(uint8_t x, uint8_t a = 3);
};
