#include "CmyConverter.hpp"

#include <memory>

void CmyConverter::ConvertPixelToRGB(Pixel& pixel)
{
    for (int i = 0; i < 3; i++) {
        pixel.channels[i] = 1.0f - pixel.channels[i];
    }
}

void CmyConverter::ConvertPixelFromRGB(Pixel& pixel)
{
    ConvertPixelToRGB(pixel); // symmetric
}