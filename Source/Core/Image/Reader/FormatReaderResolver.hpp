#pragma once

#include "ImageReader.hpp"

#include <memory>

class FormatReaderResolver
{
public:
    static std::shared_ptr<ImageReader> GetReader(const std::vector<uint8_t>& data);
private:
    FormatReaderResolver() = default;
};
