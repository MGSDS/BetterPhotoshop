#include "OrderedDither.hpp"

#include <stdexcept>
#include <vector>

static const size_t N = 8;

static const float TRESHOLDS[N][N] = {
    { 0, 32, 8, 40, 2, 34, 10, 42 },
    { 48, 16, 56, 24, 50, 18, 58, 26 },
    { 12, 44, 4, 36, 14, 46, 6, 38 },
    { 60, 28, 52, 20, 62, 30, 54, 22 },
    { 3, 35, 11, 43, 1, 33, 9, 41 },
    { 51, 19, 59, 27, 49, 17, 57, 25 },
    { 15, 47, 7, 39, 13, 45, 5, 37 },
    { 63, 31, 55, 23, 61, 29, 53, 21 }
};

std::unique_ptr<Image> OrderedDither::Apply(const Image& image, uint8_t bitsPerChannel)
{
    if (bitsPerChannel == 0 || bitsPerChannel > 8) {
        throw new std::runtime_error("Maximum bit depth () exceeded.");
    }

    float quantSize = 1.0f / ((1u << bitsPerChannel) - 1);

    std::unique_ptr<Image> newImage = std::make_unique<Image>(image);

    for (int i = 0; i < newImage->GetHeight(); i++) {
        for (int j = 0; j < newImage->GetWidth(); j++) {
            float diff = quantSize * (TRESHOLDS[i % N][j % N] / (N * N) - 0.5f);
            Pixel& pixel = newImage->PixelAt(i, j);
            for (size_t k = 0; k < 3; k++) {
                pixel.channels[k] += diff;
            }
            Dither::QuantizePixel(pixel, bitsPerChannel);
        }
    }

    return newImage;
}