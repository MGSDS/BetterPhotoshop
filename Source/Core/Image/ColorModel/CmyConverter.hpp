#pragma once

#include "ColorModelConverter.hpp"

class CmyConverter : public ColorModelConverter
{
public:
    ~CmyConverter() override = default;

protected:
    void ConvertPixelToRGB(Pixel& pixel) override;
    void ConvertPixelFromRGB(Pixel& pixel) override;
};