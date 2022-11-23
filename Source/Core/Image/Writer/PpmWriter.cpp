#include "PpmWriter.hpp"
#include "Core/Utils/Utils.hpp"

void PpmWriter::WriteHeader(const Image& image, std::ostream& os)
{
    os << "P6\n";
    os << image.GetWidth() << ' ' << image.GetHeight() << '\n';
    os << 255 << '\n';
}

void PpmWriter::Write(const Image& image, std::ostream& os, uint8_t bitsPerChannel) const
{
    WriteHeader(image, os);
    for (size_t i = 0; i < image.GetPixelsCount(); i++) {
        auto& pixel = image.PixelAt(i);
        for (size_t j = 0; j < 3; j++) {
            os.put(Utils::ByteFromNorm(pixel.channels[j]));
        }
    }
    os.flush();
}