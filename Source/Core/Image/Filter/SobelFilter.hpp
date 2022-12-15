#pragma once

#include "Filter.hpp"

class SobelFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    SobelFilter() = default;
    virtual ~SobelFilter() override = default;

private:
    static Pixel GetPixel(const Image& image, int x, int y);
};
