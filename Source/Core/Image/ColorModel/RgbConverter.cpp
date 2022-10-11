#include "RgbConverter.hpp"

#include <memory>

std::unique_ptr<Image> RgbConverter::ToRGB(const Image& origin)
{
    return std::make_unique<Image>(origin);
}

std::unique_ptr<Image> RgbConverter::FromRGB(const Image& origin)
{
    return std::make_unique<Image>(origin);
}

void RgbConverter::ConvertPixelToRGB(Pixel& pixel)
{
}

void RgbConverter::ConvertPixelFromRGB(Pixel& pixel)
{
}