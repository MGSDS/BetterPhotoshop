#pragma once

#include "ColorModelConverter.hpp"

class HsvConverter : ColorModelConverter {
protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};
