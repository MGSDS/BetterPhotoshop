#pragma once

#include "Dither.hpp"

class TresholdDither : public Dither
{
public:
    std::unique_ptr<Image> Apply(const Image& image, uint8_t bitsPerChannel) override;
    ~TresholdDither() override = default;
};