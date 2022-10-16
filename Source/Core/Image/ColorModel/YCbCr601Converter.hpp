#pragma once

#include "ColorModelConverter.hpp"

class YCbCr601Converter : ColorModelConverter {
protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};

