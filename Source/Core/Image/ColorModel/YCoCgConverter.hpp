#pragma once

#include "ColorModelConverter.hpp"

class YCoCgConverter : public ColorModelConverter {
public:
    ~YCoCgConverter() override = default;

protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};

