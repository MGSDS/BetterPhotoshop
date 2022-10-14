#include "ImageWriter.hpp"

#include <Core/Image/Writer/PgmWriter.hpp>
#include <Core/Image/Writer/PpmWriter.hpp>

std::unique_ptr<ImageWriter> ImageWriter::GetWriter(ImageFormat format)
{
    switch (format) {
        case ImageFormat::Pgm: return std::make_unique<PgmWriter>();
        case ImageFormat::Ppm: return std::make_unique<PpmWriter>();
        default: break;
    }

    return nullptr;
}
