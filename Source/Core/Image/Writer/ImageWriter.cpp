#include "ImageWriter.hpp"

#include <Core/Image/Writer/PgmWriter.hpp>
#include <Core/Image/Writer/PpmWriter.hpp>
#include <Core/Image/Writer/PngWriter.h>

std::unique_ptr<ImageWriter> ImageWriter::GetWriter(ImageFormat format)
{
    switch (format) {
        case ImageFormat::Pgm: return std::make_unique<PgmWriter>();
        case ImageFormat::Ppm: return std::make_unique<PpmWriter>();
        case ImageFormat::Png: return std::make_unique<PngWriter>();
        default: break;
    }

    return nullptr;
}
