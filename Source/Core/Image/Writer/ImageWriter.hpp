#pragma once

#include "Core/Image/Image.hpp"

#include <ostream>


class ImageWriter
{
public:
    virtual void Write(const Image& image, std::ostream& os) const = 0;

protected:
    ImageWriter() = default;
    virtual ~ImageWriter() = default;
};