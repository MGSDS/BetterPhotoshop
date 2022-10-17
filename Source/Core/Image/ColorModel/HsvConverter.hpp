#pragma once

#include "ColorModelConverter.hpp"

class HsvConverter : public ColorModelConverter
{
public:
    ~HsvConverter() override = default;

protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};
