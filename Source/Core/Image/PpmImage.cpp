#include "PpmImage.hpp"
#include "Core/Image/Writer/PpmWriter.hpp"

#include <fstream>


void PpmImage::WriteToFile(const std::string& fileName) const
{
    std::ofstream ofs(fileName, std::ofstream::out);
    PpmWriter writer;
    writer.Write(*this, ofs);
    ofs.close();
}

PpmImage::PpmImage(size_t width, size_t height, std::vector<Pixel>& pixels) : Image(width, height, pixels) {}

std::string PpmImage::GetExtension() const {
    return ".ppm";
}

PpmImage::PpmImage(size_t width, size_t height) : Image(width, height) {
}
