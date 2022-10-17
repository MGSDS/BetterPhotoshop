#pragma once

#include "ColorModelConverter.hpp"

class YCbCr601Converter : public ColorModelConverter {
public:
    ~YCbCr601Converter() override = default;

protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};

