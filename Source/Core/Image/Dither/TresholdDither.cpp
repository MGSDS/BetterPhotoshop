#include "TresholdDither.hpp"

#include "Core/Utils/Utils.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <random>

static float GetRandomInt(int maxValue) {
    return rand() % maxValue;
}


std::unique_ptr<Image> TresholdDither::Apply(const Image& image, uint8_t bitsPerChannel)
{
    if (bitsPerChannel == 0 || bitsPerChannel > 8) {
        throw new std::runtime_error("Maximum bit depth () exceeded.");
    }

    std::unique_ptr<Image> newImage = std::make_unique<Image>(image);
    for (size_t i = 0; i < newImage->GetPixelsCount(); i++) {
        Dither::QuantizePixel(newImage->PixelAt(i), bitsPerChannel);
    }

    return newImage;
}