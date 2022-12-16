#include "PngWriter.hpp"
#include "sstream"
#include "zlib.h"
#include <Core/Utils/Utils.hpp>

#define IHDR \
    std::vector<uint8_t> { 73, 72, 68, 82 }
#define IDAT \
    std::vector<uint8_t> { 73, 68, 65, 84 }
#define IEND \
    std::vector<uint8_t> { 73, 69, 78, 68 }
#define PNG \
    std::vector<uint8_t> { 137, 80, 78, 71, 13, 10, 26, 10 }
#define gAMA \
    std::vector<uint8_t> { 103, 65, 77, 65 }

void PngWriter::Write(const Image& image, std::ostream& os, uint8_t bitsPerChannel, bool grayscale) const
{
    os << PNG[0] << PNG[1] << PNG[2] << PNG[3] << PNG[4] << PNG[5] << PNG[6] << PNG[7];
    WriteIHDR(image, os, grayscale);
    if (image.GetGamma() != 1.0f) {
        WriteGamma(image, os);
    }

    WriteData(image, os, grayscale);
    WriteFooter(os);
}

void PngWriter::WriteFooter(std::ostream& stream)
{
    uint8_t size = 0;
    stream << size << size << size << size;
    stream << IEND[0] << IEND[1] << IEND[2] << IEND[3];
    stream << static_cast<uint8_t>(0xAE) << static_cast<uint8_t>(0x42) << static_cast<uint8_t>(0x60) << static_cast<uint8_t>(0x82);
}

void PngWriter::WriteChunk(const std::vector<uint8_t>& chunkType, const std::vector<uint8_t>& data, std::ostream& stream)
{
    size_t size = data.size();
    stream << static_cast<char>(size >> 24) << static_cast<char>(size >> 16) << static_cast<char>(size >> 8) << static_cast<char>(size);
    stream << chunkType[0] << chunkType[1] << chunkType[2] << chunkType[3];
    stream.write(reinterpret_cast<const char*>(data.data()), data.size());
    std::vector<uint8_t> crcData;
    crcData.insert(crcData.end(), chunkType.begin(), chunkType.end());
    crcData.insert(crcData.end(), data.begin(), data.end());
    uint32_t crc = Utils::Crc32(crcData.begin(), crcData.end());
    stream << static_cast<char>(crc >> 24) << static_cast<char>(crc >> 16) << static_cast<char>(crc >> 8) << static_cast<char>(crc);
}

void PngWriter::WriteIHDR(const Image& image, std::ostream& stream, bool grayscale)
{
    std::vector<uint8_t> data;
    uint32_t width = image.GetWidth();
    uint32_t height = image.GetHeight();
    uint32_t size = 13;
    data.push_back(width >> 24);
    data.push_back(width >> 16);
    data.push_back(width >> 8);
    data.push_back(width);
    data.push_back(height >> 24);
    data.push_back(height >> 16);
    data.push_back(height >> 8);
    data.push_back(height);
    data.push_back(8);
    if (!grayscale) {
        data.push_back(2);
    } else {
        data.push_back(0);
    }
    data.push_back(0);
    data.push_back(0);
    data.push_back(0);
    WriteChunk(IHDR, data, stream);
}

void PngWriter::WriteData(const Image& image, std::ostream& stream, bool grayscale)
{
    std::vector<uint8_t> data;
    uint32_t width = image.GetWidth();
    uint32_t height = image.GetHeight();
    for (uint32_t i = 0; i < height; i++) {
        data.push_back(0);
        for (uint32_t j = 0; j < width; j++) {
            Pixel pixel = image.PixelAt(i, j);
            uint8_t r = Utils::ByteFromNorm(pixel.channels[0]);
            uint8_t g = Utils::ByteFromNorm(pixel.channels[1]);
            uint8_t b = Utils::ByteFromNorm(pixel.channels[2]);
            if (grayscale) {
                data.push_back((r + g + b) / 3);
            } else {
                data.push_back(r);
                data.push_back(g);
                data.push_back(b);
            }
        }
    }
    data = Deflate(data);
    WriteChunk(IDAT, data, stream);
}

std::vector<uint8_t> PngWriter::Deflate(const std::vector<uint8_t>& data)
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = data.size();
    stream.next_in = const_cast<uint8_t*>(data.data());
    stream.avail_out = 0;
    stream.next_out = Z_NULL;
    int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        throw std::runtime_error("deflateInit failed");
    }
    std::vector<uint8_t> compressedData;
    do {
        compressedData.resize(compressedData.size() + 1024);
        stream.next_out = compressedData.data() + stream.total_out;
        stream.avail_out = compressedData.size() - stream.total_out;
        ret = deflate(&stream, Z_FINISH);
    } while (ret == Z_OK);
    compressedData.resize(stream.total_out);
    deflateEnd(&stream);
    return compressedData;
}

void PngWriter::WriteGamma(const Image& image, std::ostream& stream)
{
    std::vector<uint8_t> data;
    uint32_t gamma = 1.0f / image.GetGamma() * 100000;
    data.push_back(gamma >> 24);
    data.push_back(gamma >> 16);
    data.push_back(gamma >> 8);
    data.push_back(gamma);
    WriteChunk(gAMA, data, stream);
}
