#include "PgmImage.hpp"
#include "Core/Image/Writer/PgmWriter.hpp"

#include <fstream>

void PgmImage::WriteToFile(const std::string& fileName) const
{
    std::ofstream ofs(fileName, std::ofstream::out);
    PgmWriter writer;
    writer.Write(*this, ofs);
    ofs.close();
}

PgmImage::PgmImage(size_t width, size_t height, std::vector<Pixel>& pixels) : Image(width, height, pixels) {}
