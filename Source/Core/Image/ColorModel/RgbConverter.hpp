#pragma once

#include "ColorModelConverter.hpp"

class RgbConverter : public ColorModelConverter
{
public:
    std::unique_ptr<Image> ToRGB(const Image& origin) override;
    std::unique_ptr<Image> FromRGB(const Image& origin) override;

    ~RgbConverter() override = default;

protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};