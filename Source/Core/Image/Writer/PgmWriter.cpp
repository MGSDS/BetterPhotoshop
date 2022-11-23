#include "PgmWriter.hpp"

#include "Core/Image/Dither/Dither.hpp"
#include "Core/Utils/Utils.hpp"

#include <string>
#include <cmath>

void PgmWriter::WriteHeader(const Image& image, std::ostream& os)
{
    os << "P5\n";
    os << image.GetWidth() << ' ' << image.GetHeight() << '\n';
    os << 255 << '\n';
}

void PgmWriter::Write(const Image& image, std::ostream& os, uint8_t bitsPerChannel) const
{
    WriteHeader(image, os);
    for (size_t i = 0; i < image.GetPixelsCount(); i++) {
        auto& pixel = image.PixelAt(i);

        float meanChannelValue = (pixel.channels[0] +
                                  pixel.channels[1] +
                                  pixel.channels[2]) /
                                 3.0f;
        
        os.put(Utils::ByteFromNorm(meanChannelValue));
    }
    os.flush();
}