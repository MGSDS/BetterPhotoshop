#include "Dither.hpp"

#include "AtkinsonDither.hpp"
#include "FloydSteinbergDither.hpp"
#include "OrderedDither.hpp"
#include "RandomDither.hpp"
#include "TresholdDither.hpp"

#include "Core/Utils/Utils.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

std::unique_ptr<Dither> Dither::GetDither(DitherAlgo ditherAlgo)
{
    switch (ditherAlgo) {
        case DitherAlgo::ATKINSON: return std::make_unique<AtkinsonDither>();
        case DitherAlgo::FLOYD_STEINBERG: return std::make_unique<FloydSteinbergDither>();
        case DitherAlgo::ORDERED: return std::make_unique<OrderedDither>();
        case DitherAlgo::RANDOM: return std::make_unique<RandomDither>();
        case DitherAlgo::TRESHOLD: return std::make_unique<TresholdDither>();
        default: throw std::runtime_error("Unexpected DitherAlgo value.");
    }
}

float Dither::GetQuantSize(uint8_t bitsPerChannel)
{
    return 1.0f / ((1u << bitsPerChannel) - 1);
}

void Dither::QuantizeChannel(float& value, uint8_t bitsPerChannel)
{
    float floatQuantSize = Dither::GetQuantSize(bitsPerChannel);
    value = std::clamp(floatQuantSize * std::round(value / floatQuantSize), 0.0f, 1.0f);
}

void Dither::QuantizePixel(Pixel& pixel, uint8_t bitsPerChannel)
{
    for (size_t i = 0; i < 3; i++) {
        Dither::QuantizeChannel(pixel.channels[i], bitsPerChannel);
    }
}

void Dither::SetErrors(float errors[3], const Pixel& prev, const Pixel& quantized)
{
    for (size_t i = 0; i < 3; i++) {
        errors[i] = prev.channels[i] - quantized.channels[i];
    }
}

void Dither::AddErrors(Pixel& pixel, float errors[3], float weight)
{
    for (size_t i = 0; i < 3; i++) {
        pixel.channels[i] += errors[i] * weight;
    }
}

void Dither::QuantizeWithErrorsSpreading(
    Image& image,
    uint8_t bitsPerChannel,
    std::vector<std::tuple<std::pair<int32_t, int32_t>, float>> traverse)
{
    size_t height = image.GetHeight();
    size_t width = image.GetWidth();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel prev = image.PixelAt(i, j); // copy!
            Dither::QuantizePixel(image.PixelAt(i, j), bitsPerChannel);
            float errors[3];
            Dither::SetErrors(errors, prev, image.PixelAt(i, j));
            for (const auto& step : traverse) {
                auto [iOffset, jOffset] = std::get<0>(step);
                float quotient = std::get<1>(step);
                int iWithOffset = i + iOffset;
                int jWithOffset = j + jOffset;

                if (Utils::OutOfBounds(iWithOffset, jWithOffset, height, width)) {
                    continue;
                }

                Pixel& nextPixel = image.PixelAt(iWithOffset, jWithOffset);
                Dither::AddErrors(nextPixel, errors, quotient);
            }
        }
    }
}