#pragma once

#include "Core/Image/Image.hpp"

#include <memory>

class ImageReader
{
public:
    virtual std::shared_ptr<Image> ReadImage(const std::vector<uint8_t>& data) = 0;

protected:
    ImageReader() = default;
    virtual ~ImageReader() = default;
};
