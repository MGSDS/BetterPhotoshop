#pragma once

#include "Dither.hpp"

class RandomDither : public Dither
{
public:
    std::unique_ptr<Image> Apply(const Image& image, uint8_t bitsPerChannel) override;
    ~RandomDither() override = default;
};