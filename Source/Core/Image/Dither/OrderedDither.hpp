#pragma once

#include "Dither.hpp"

class OrderedDither : public Dither
{
public:
    std::unique_ptr<Image> Apply(const Image& image, uint8_t bitsPerChannel) override;
    ~OrderedDither() override = default;
};