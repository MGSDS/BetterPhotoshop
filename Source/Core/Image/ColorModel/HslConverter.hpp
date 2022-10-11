#pragma once

#include "ColorModelConverter.hpp"

class HslConverter : public ColorModelConverter
{
public:
    ~HslConverter() override = default;

protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};