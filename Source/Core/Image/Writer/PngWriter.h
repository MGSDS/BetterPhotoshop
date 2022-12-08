#pragma once

#include "ImageWriter.hpp"

class PngWriter : public ImageWriter
{
public:
    void Write(const Image& image, std::ostream& os, uint8_t bitsPerChannel = 8, bool grayscale = false) const override;

private:
    static void WriteIHDR(const Image& image, std::ostream& stream, bool grayscale);
    static void WriteData(const Image& image, std::ostream& stream, bool grayscale);
    static void WriteFooter(std::ostream& stream);
    static void WriteChunk(const std::vector<uint8_t>& chunkType, const std::vector<uint8_t>& data, std::ostream& stream);
    static std::vector<uint8_t> Deflate(const std::vector<uint8_t>& data);
};
