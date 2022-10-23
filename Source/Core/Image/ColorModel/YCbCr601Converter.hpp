#pragma once

#include "YCbCrConverter.hpp"

class YCbCr601Converter : public YCbCrConverter
{
public:
    YCbCr601Converter()
        : YCbCrConverter(0.114, 0.299){};
};
