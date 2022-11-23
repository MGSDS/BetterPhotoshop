#pragma once

#include "Core/Image/Image.hpp"

#include <cstdint>
#include <memory>
#include <unordered_map>

enum DitherAlgo : uint8_t
{
    ATKINSON,
    FLOYD_STEINBERG,
    ORDERED,
    RANDOM,
    TRESHOLD
};

static const std::unordered_map<DitherAlgo, std::string> ENUM_TO_STRING_DITHER_ALGO_MAPPING = {
    { DitherAlgo::ATKINSON, "Atkinson" },
    { DitherAlgo::FLOYD_STEINBERG, "Floyd-Steinberg" },
    { DitherAlgo::ORDERED, "Ordered (8x8)" },
    { DitherAlgo::RANDOM, "Random" },
    { DitherAlgo::TRESHOLD, "Treshold" }
};

class Dither
{
public:
    static std::unique_ptr<Dither> GetDither(DitherAlgo ditherAlgo);
    virtual std::unique_ptr<Image> Apply(const Image& image, uint8_t depth) = 0;
    virtual ~Dither() = default;

    // utils
    static float GetQuantSize(uint8_t bitsPerChannel);
    static void QuantizeChannel(float& value, uint8_t bitsPerChannel);
    static void QuantizePixel(Pixel& pixel, uint8_t bitsPerChannel);
    static void SetErrors(float errors[3], const Pixel& prev, const Pixel& quantized);
    static void AddErrors(Pixel& pixel, float errors[3], float weight);
    static void QuantizeWithErrorsSpreading(
        Image& image,
        uint8_t bitsPerChannel,
        std::vector<std::tuple<std::pair<int32_t, int32_t>, float>> traverse);
};