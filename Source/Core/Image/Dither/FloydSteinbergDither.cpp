#include "FloydSteinbergDither.hpp"

#include "Core/Utils/Utils.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <vector>

const std::vector<std::tuple<std::pair<int32_t, int32_t>, float>> FloydSteinbergDither::TRAVERSE = {
    { { 0, 1 }, 7.0f / 16 },
    { { 1, -1 }, 3.0f / 16 },
    { { 1, 0 }, 5.0f / 16 },
    { { 1, 1 }, 1.0f / 16 }
};

std::unique_ptr<Image> FloydSteinbergDither::Apply(const Image& image, uint8_t bitsPerChannel)
{
    if (bitsPerChannel == 0 || bitsPerChannel > 8) {
        throw new std::out_of_range("Specified bitsPerChannel value is out of range.");
    }

    std::unique_ptr<Image> newImage = std::make_unique<Image>(image);

    Dither::QuantizeWithErrorsSpreading(*newImage, bitsPerChannel, FloydSteinbergDither::TRAVERSE);

    return newImage;
}