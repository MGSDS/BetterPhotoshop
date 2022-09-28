#include "ImageReader.hpp"

#include <Core/Log.hpp>
#include <Core/Utils/Utils.hpp>
#include <Core/Image/Reader/PgmReader.hpp>
#include <Core/Image/Reader/PpmReader.hpp>
#include <Core/Image/ImageFormat.hpp>

#include <unordered_map>

static std::unordered_map<std::string, ImageFormat> EXTENSION_TO_IMAGE_FORMAT = {
    { ".pgm", ImageFormat::Pgm },
    { ".ppm", ImageFormat::Ppm },
};

std::unique_ptr<ImageReader> ImageReader::GetReader(const std::string& fileExtension)
{
    std::string extension = Utils::ToLower(fileExtension);
 
    auto format = EXTENSION_TO_IMAGE_FORMAT.find(extension);
    if (format == EXTENSION_TO_IMAGE_FORMAT.end()) {
        return nullptr;
    }

    switch(format->second) {
        case ImageFormat::Pgm:  return std::make_unique<PgmReader>();
        case ImageFormat::Ppm:  return std::make_unique<PpmReader>();
        default: break;
    }

    return nullptr;
}
