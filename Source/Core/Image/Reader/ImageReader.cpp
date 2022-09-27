#include "ImageReader.hpp"

#include <Core/Log.hpp>
#include <Core/Utils/Utils.hpp>
#include <Core/Image/Reader/PgmReader.hpp>
#include <Core/Image/Reader/PpmReader.hpp>

std::unique_ptr<ImageReader> ImageReader::GetReader(const std::string& fileExtension)
{
    std::string extension = Utils::ToLower(fileExtension);
    
    if (extension == ".pgm") {
        return std::make_unique<PgmReader>();
    }
    if (extension == ".ppm") {
        return std::make_unique<PpmReader>();
    }

    return nullptr;
}
