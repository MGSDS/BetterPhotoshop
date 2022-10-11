#pragma once

#include "Core/Image/Image.hpp"

enum ColorModel
{
    RGB = 0,
    CMY,
    HSL
};

class ColorModelConverter
{
public:
    static std::unique_ptr<ColorModelConverter> GetConverter(ColorModel colorModel);
    virtual std::unique_ptr<Image> ToRGB(const Image& origin);
    virtual std::unique_ptr<Image> FromRGB(const Image& origin);

    virtual ~ColorModelConverter() = default;

protected:
    virtual void ConvertPixelToRGB(Pixel& pixel) = 0;
    virtual void ConvertPixelFromRGB(Pixel& pixel) = 0;
};