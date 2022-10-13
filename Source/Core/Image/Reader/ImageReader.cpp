#include "ImageReader.hpp"

#include <Core/Log.hpp>
#include <Core/Utils/Utils.hpp>
#include <Core/Image/Reader/PgmReader.hpp>
#include <Core/Image/Reader/PpmReader.hpp>
#include <Core/Image/ImageFormat.hpp>

#include <unordered_map>
#include <vector>

static std::unordered_map<std::string, ImageFormat> EXTENSION_TO_IMAGE_FORMAT = {
    { ".pgm", ImageFormat::Pgm },
    { ".ppm", ImageFormat::Ppm },
};

static std::unordered_map<ImageFormat, std::vector<uint8_t>> IMAGE_FORMAT_TO_FILE_PREFIX = {
    { ImageFormat::Pgm, { (uint8_t)'P', (uint8_t)'5' } },
    { ImageFormat::Ppm, { (uint8_t)'P', (uint8_t)'6' } }
};

std::unique_ptr<ImageReader> ImageReader::GetReader(const std::string& fileExtension)
{
    std::string extension = Utils::ToLower(fileExtension);
 
    auto format = EXTENSION_TO_IMAGE_FORMAT.find(extension);
    if (format == EXTENSION_TO_IMAGE_FORMAT.end()) {
        return nullptr;
    }

    return GetReader(format->second);
}

std::unique_ptr<ImageReader> ImageReader::GetReader(const std::vector<uint8_t>& data) 
{
    if (auto imageFormat = GetImageFormat(data)) {
        return GetReader(*imageFormat);
    }

    return nullptr;
}

std::unique_ptr<ImageReader> ImageReader::GetReader(ImageFormat format)
{
    switch(format) {
        case ImageFormat::Pgm:  return std::make_unique<PgmReader>();
        case ImageFormat::Ppm:  return std::make_unique<PpmReader>();
        default: break;
    }

    return nullptr;
}

std::optional<ImageFormat> ImageReader::GetImageFormat(const std::vector<uint8_t>& data)
{
    if (Utils::DataStartsWith(data, IMAGE_FORMAT_TO_FILE_PREFIX[ImageFormat::Pgm])) {
        return ImageFormat::Pgm;
    }
    if (Utils::DataStartsWith(data, IMAGE_FORMAT_TO_FILE_PREFIX[ImageFormat::Ppm])) {
        return ImageFormat::Ppm;
    }

    return {};
}