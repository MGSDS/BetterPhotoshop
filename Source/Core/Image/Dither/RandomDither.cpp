#include "RandomDither.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <stdexcept>

static float GetRandomFloat(float maxValue)
{
    return maxValue * (rand() / (static_cast<float>(RAND_MAX)));
}

std::unique_ptr<Image> RandomDither::Apply(const Image& image, uint8_t bitsPerChannel)
{
    //Random dithering


    if (bitsPerChannel == 0 || bitsPerChannel > 8) {
        throw new std::out_of_range("Specified bitsPerChannel value is out of range.");
    }
    srand(time(0));

    float quantSize = Dither::GetQuantSize(bitsPerChannel);

    std::unique_ptr<Image> newImage = std::make_unique<Image>(image);
    for (size_t i = 0; i < newImage->GetPixelsCount(); i++) {
        Pixel& pixel = newImage->PixelAt(i);
        float diff = GetRandomFloat(quantSize) - quantSize / 2.0f;
        for (size_t j = 0; j < 3; j++) {
            pixel.channels[j] = std::clamp(quantSize * std::round((pixel.channels[j] + diff) / quantSize), 0.0f, 1.0f);
        }
    }

    return newImage;
}