#pragma once

#include "Core/Image/Image.hpp"

#include <memory>
#include <unordered_map>

enum ColorModel
{
    RGB = 0,
    CMY,
    HSL,
    HSV,
    YCbCr601,
    YCbCr709,
    YCoCg,
};

static const std::unordered_map<std::string, ColorModel> STRING_TO_ENUM_COLOR_MODEL_MAPPING = {
    { "RGB", ColorModel::RGB },
    { "CMY", ColorModel::CMY },
    { "HSL", ColorModel::HSL },
    { "HSV", ColorModel::HSV },
    { "YCbCr.601", ColorModel::YCbCr601 },
    { "YCbCr.709", ColorModel::YCbCr709 },
    { "YCoCg", ColorModel::YCoCg },
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