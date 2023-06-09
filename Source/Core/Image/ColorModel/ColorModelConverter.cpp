#include "ColorModelConverter.hpp"

#include "CmyConverter.hpp"
#include "HslConverter.hpp"
#include "HsvConverter.hpp"
#include "RgbConverter.hpp"
#include "YCbCr601Converter.hpp"
#include "YCbCr709Converter.hpp"
#include "YCoCgConverter.hpp"

#include <memory>
#include <stdexcept>

std::unique_ptr<ColorModelConverter> ColorModelConverter::GetConverter(ColorModel colorModel)
{
    switch (colorModel) {
        case ColorModel::RGB: return std::make_unique<RgbConverter>();
        case ColorModel::CMY: return std::make_unique<CmyConverter>();
        case ColorModel::HSL: return std::make_unique<HslConverter>();
        case ColorModel::HSV: return std::make_unique<HsvConverter>();
        case ColorModel::YCbCr601: return std::make_unique<YCbCr601Converter>();
        case ColorModel::YCbCr709: return std::make_unique<YCbCr709Converter>();
        case ColorModel::YCoCg: return std::make_unique<YCoCgConverter>();
        default: throw std::runtime_error("Unhandled color model");
    }
}

std::unique_ptr<Image> ColorModelConverter::ToRGB(const Image& origin)
{
    auto converted = std::make_unique<Image>(origin);
    for (size_t i = 0; i < converted->GetPixelsCount(); i++) {
        ConvertPixelToRGB(converted->PixelAt(i));
    }
    return converted;
}

std::unique_ptr<Image> ColorModelConverter::FromRGB(const Image& origin)
{
    auto converted = std::make_unique<Image>(origin);
    for (size_t i = 0; i < converted->GetPixelsCount(); i++) {
        ConvertPixelFromRGB(converted->PixelAt(i));
    }
    return converted;
}