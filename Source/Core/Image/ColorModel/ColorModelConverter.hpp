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

static const std::unordered_map<ColorModel, std::string> ENUM_TO_STRING_COLOR_MODEL_MAPPING = {
    { ColorModel::RGB, "RGB" },
    { ColorModel::CMY, "CMY" },
    { ColorModel::HSL, "HSL" },
    { ColorModel::HSV, "HSV" },
    { ColorModel::YCbCr601, "YCbCr.601" },
    { ColorModel::YCbCr709, "YCbCr.709" },
    { ColorModel::YCoCg, "YCoCg" },
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