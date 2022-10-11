#include "PgmWriter.hpp"
#include "Core/Utils/Utils.hpp"

#include <string>


void PgmWriter::WriteHeader(const Image& image, std::ostream& os)
{
    os << "P5\n";
    os << image.GetWidth() << ' ' << image.GetHeight() << '\n';
    os << 255 << '\n';
}

void PgmWriter::Write(const Image& image, std::ostream& os) const
{
    WriteHeader(image, os);
    for (size_t i = 0; i < image.GetPixelsCount(); i++)
    {
        auto& pixel = image.PixelAt(i);

        float meanChannelValue = (
            pixel.channels[0] + 
            pixel.channels[1] +
            pixel.channels[2]
        ) / 3.0f;
        
        os.put(Utils::ByteFromNorm(meanChannelValue));
    }
    os.flush();
}