#pragma once

#include "ColorModelConverter.hpp"

class YCbCrConverter : public ColorModelConverter
{
public:
    YCbCrConverter(float Kb, float Kr);
    ~YCbCrConverter() override = default;

protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
    float Kb;
    float Kr;
};
