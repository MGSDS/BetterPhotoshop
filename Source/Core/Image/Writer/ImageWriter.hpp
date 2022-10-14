#pragma once

#include <Core/Image/Image.hpp>
#include <Core/Image/ImageFormat.hpp>

#include <memory>
#include <ostream>

class ImageWriter
{
public:
    virtual ~ImageWriter() = default;

    static std::unique_ptr<ImageWriter> GetWriter(ImageFormat format);

    virtual void Write(const Image& image, std::ostream& os) const = 0;
};
