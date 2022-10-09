#pragma once

#include <Core/Image/Image.hpp>
#include <Core/Image/ImageFormat.hpp>

#include <memory>
#include <vector>

class ImageReader
{
public:
    virtual ~ImageReader() = default;

    static std::unique_ptr<ImageReader> GetReader(const std::string& fileExtension);

    static std::unique_ptr<ImageReader> GetReader(const std::vector<uint8_t>& data);
    
    virtual std::unique_ptr<Image> ReadImage(const std::vector<uint8_t>& data) = 0;
};
