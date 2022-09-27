#pragma once

#include <Core/Image/Image.hpp>
#include <Core/Image/ImageFormat.hpp>

#include <memory>

class ImageReader
{
public:
    virtual ~ImageReader() = default;

    static std::unique_ptr<ImageReader> GetReader(const std::string& fileExtension);    
    
    virtual std::shared_ptr<Image> ReadImage(const std::vector<uint8_t>& data) = 0;
};
