#pragma once

#include "Dither.hpp"

class AtkinsonDither : public Dither
{
public:
    std::unique_ptr<Image> Apply(const Image& image, uint8_t bitsPerChannel) override;
    ~AtkinsonDither() override = default;

private:
    static const std::vector<std::tuple<std::pair<int32_t, int32_t>, float>> TRAVERSE;
};