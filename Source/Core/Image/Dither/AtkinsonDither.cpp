#include "AtkinsonDither.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <vector>

const std::vector<std::tuple<std::pair<int32_t, int32_t>, float>> AtkinsonDither::TRAVERSE = {
    { { 0, 1 }, 0.125f },
    { { 0, 2 }, 0.125f },
    { { 1, -1 }, 0.125f },
    { { 1, 0 }, 0.125f },
    { { 1, 1 }, 0.125f },
    { { 2, 0 }, 0.125f }
};

std::unique_ptr<Image> AtkinsonDither::Apply(const Image& image, uint8_t bitsPerChannel)
{
    if (bitsPerChannel == 0 || bitsPerChannel > 8) {
        throw new std::out_of_range("Specified bitsPerChannel value is out of range.");
    }

    std::unique_ptr<Image> newImage = std::make_unique<Image>(image);

    Dither::QuantizeWithErrorsSpreading(*newImage, bitsPerChannel, AtkinsonDither::TRAVERSE);

    return newImage;
}