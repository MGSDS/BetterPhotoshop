#pragma once

#include "YCbCrConverter.hpp"

class YCbCr709Converter : public YCbCrConverter
{
public:
    YCbCr709Converter()
        : YCbCrConverter(0.0722, 0.2126) {}
};
