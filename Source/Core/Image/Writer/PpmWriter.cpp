#include "PpmWriter.hpp"


void PpmWriter::WriteHeader(const Image& image, std::ostream& os)
{
    os << "P6\n";
    os << image.GetWidth() << ' ' << image.GetHeight() << '\n';
    os << 255 << '\n';
}

void PpmWriter::Write(const Image& image, std::ostream& os) const
{
    WriteHeader(image, os);
    for (size_t i = 0; i < image.GetPixelsCount(); i++)
    {
        auto& pixel = image.PixelAt(i);
        os.put(pixel.red).put(pixel.green).put(pixel.blue);
    }
    os.flush();
}