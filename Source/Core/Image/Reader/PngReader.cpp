#include "PngReader.h"
#include "zlib.h"
#include <Core/Utils/Utils.hpp>
#include <algorithm>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>

std::unique_ptr<Image> PngReader::ReadImage(const std::vector<uint8_t>& data)
{
    if (!IsPngHeader(data)) {
        throw std::invalid_argument("Invalid PNG header");
    }
    std::vector<Chunk> chunks = ReadChunks(data);

    Chunk ihdr = chunks[0];
    if (ihdr.type != 0x49484452) {
        throw std::invalid_argument("Invalid PNG");
    }

    PngHeader header{
        .width = static_cast<uint32_t>((ihdr.data[0] << 24) | (ihdr.data[1] << 16) | (ihdr.data[2] << 8) | ihdr.data[3]),
        .height = static_cast<uint32_t>((ihdr.data[4] << 24) | (ihdr.data[5] << 16) | (ihdr.data[6] << 8) | ihdr.data[7]),
        .bitDepth = ihdr.data[8],
        .colorType = ihdr.data[9],
        .compressionMethod = ihdr.data[10],
        .filterMethod = ihdr.data[11],
        .interlaceMethod = ihdr.data[12]
    };

    if (header.bitDepth != 8) {
        throw std::invalid_argument("Only 8-bit PNGs are supported");
    }

    if (header.interlaceMethod != 0) {
        throw std::invalid_argument("Interlaced PNGs are not supported");
    }

    if (!header.Validate()) {
        throw std::invalid_argument("Invalid PNG header");
    }

    Chunk iend = chunks[chunks.size() - 1];
    if (iend.type != 0x49454E44) {
        throw std::invalid_argument("Invalid PNG");
    }

    std::vector<plteChunk> palette;

    for (int i = 1; i < chunks.size() - 1; i++) {
        if (chunks[i].type == 0x504C5445) {
            for (int j = 0; j < chunks[i].data.size(); j += 3) {
                plteChunk color{
                    chunks[i].data[j],
                    chunks[i].data[j + 1],
                    chunks[i].data[j + 2]
                };
                palette.push_back(color);
            }
            break;
        }
    }

    if (header.colorType == 3 && palette.empty()) {
        throw std::invalid_argument("Invalid PNG");
    }
    std::vector<uint8_t> rawImg;

    for (int i = 1; i < chunks.size() - 1; i++) {
        if (chunks[i].type == 0x49444154) {
            rawImg.insert(rawImg.end(), chunks[i].data.begin(), chunks[i].data.end());
        }
    }

    if (rawImg.empty()) {
        throw std::invalid_argument("Invalid PNG");
    }

    rawImg = Inflate(rawImg);

    auto image = DecodeImage(rawImg, header, palette);
    return image;
}

bool PngReader::IsPngHeader(const std::vector<uint8_t>& data)
{
    if (data.size() <= 8) {
        return false;
    }

    return data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47 && data[4] == 0x0D && data[5] == 0x0A && data[6] == 0x1A && data[7] == 0x0A;
}

std::vector<Chunk> PngReader::ReadChunks(const std::vector<uint8_t>& data)
{
    std::vector<Chunk> chunks;
    size_t pos = 8;
    while (pos < data.size()) {
        Chunk chunk;
        chunk.length = data[pos] << 24 | data[pos + 1] << 16 | data[pos + 2] << 8 | data[pos + 3];
        chunk.type = data[pos + 4] << 24 | data[pos + 5] << 16 | data[pos + 6] << 8 | data[pos + 7];
        chunk.data = std::vector<uint8_t>(data.begin() + pos + 8, data.begin() + pos + 8 + chunk.length);
        chunk.crc = data[pos + 8 + chunk.length] << 24 | data[pos + 8 + chunk.length + 1] << 16 | data[pos + 8 + chunk.length + 2] << 8 | data[pos + 8 + chunk.length + 3];
        chunks.push_back(chunk);
        pos += 12 + chunk.length;
    }
    return chunks;
}

std::unique_ptr<Image> PngReader::DecodeImage(const std::vector<uint8_t>& rawImg, const PngHeader& header,
                                              const std::vector<plteChunk>& palette)
{
    switch (header.colorType) {
        case 0:
            return DecodeGrayscaleImage(DecodeScanlines(rawImg, 1, header.width, header.height), header);
        case 2:
            return DecodeRGBImage(DecodeScanlines(rawImg, 3, header.width, header.height), header, palette);
        case 3:
            return DecodePaletteImage(DecodeScanlines(rawImg, 1, header.width, header.height), header, palette);
        case 4:
            return DecodeGrayscaleAlphaImage(DecodeScanlines(rawImg, 2, header.width, header.height), header);
        case 6:
            return DecodeRGBAImage(DecodeScanlines(rawImg, 4, header.width, header.height), header, palette);
        default:
            throw std::invalid_argument("Invalid PNG");
    }
}

std::unique_ptr<Image> PngReader::DecodeGrayscaleImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader& header)
{
    std::vector<Pixel> pixels;

    for (int i = 0; i < header.height; i++) {
        for (int j = 0; j < header.width; ++j) {
            float px = Utils::NormByte(scanlines[i][j], 255);
            Pixel pixel = Pixel(px, px, px, 1.0f);
            pixels.push_back(pixel);
        }
    }
    return std::make_unique<Image>(header.width, header.height, pixels);
}

std::unique_ptr<Image>
PngReader::DecodeRGBImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader& header, const std::vector<plteChunk>& palette)
{
    std::vector<Pixel> pixels;

    for (int i = 0; i < header.height; i++) {
        for (int j = 0; j < header.width; ++j) {
            float r = Utils::NormByte(scanlines[i][j * 3], 255);
            float g = Utils::NormByte(scanlines[i][j * 3 + 1], 255);
            float b = Utils::NormByte(scanlines[i][j * 3 + 2], 255);
            Pixel pixel = Pixel(r, g, b, 1.0f);
            pixels.push_back(pixel);
        }
    }
    return std::make_unique<Image>(header.width, header.height, pixels);
}

std::unique_ptr<Image>
PngReader::DecodePaletteImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader& header, const std::vector<plteChunk>& palette)
{
    std::vector<Pixel> pixels;

    for (int i = 0; i < header.height; i++) {
        for (int j = 0; j < header.width; ++j) {
            uint8_t px = scanlines[i][j];
            float r = Utils::NormByte(palette[px].red, 255);
            float g = Utils::NormByte(palette[px].green, 255);
            float b = Utils::NormByte(palette[px].blue, 255);
            Pixel pixel = Pixel(r, g, b, 1.0f);
            pixels.push_back(pixel);
        }
    }
    return std::make_unique<Image>(header.width, header.height, pixels);
}

std::unique_ptr<Image> PngReader::DecodeGrayscaleAlphaImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader& header)
{
    throw std::invalid_argument("Grayscale with alpha not implemented");
    //не нужно реализовывать
}

std::unique_ptr<Image>
PngReader::DecodeRGBAImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader& header, const std::vector<plteChunk>& palette)
{
    throw std::invalid_argument("RGBA not implemented");
    //не нужно реализовывать
}

std::vector<uint8_t> PngReader::Inflate(const std::vector<uint8_t>& data)
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;

    if (inflateInit(&stream) != Z_OK) {
        throw std::invalid_argument("Invalid PNG");
    }

    stream.avail_in = data.size();
    stream.next_in = const_cast<uint8_t*>(data.data());

    std::vector<uint8_t> result;
    do {
        result.resize(result.size() + 1024);
        stream.avail_out = 1024;
        stream.next_out = result.data() + result.size() - 1024;
        inflate(&stream, Z_NO_FLUSH);
    } while (stream.avail_out == 0);

    result.resize(result.size() - stream.avail_out);
    inflateEnd(&stream);

    return result;
}

std::vector<uint8_t> PngReader::Unfilter(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint8_t filter, uint32_t width, uint8_t bpp)
{
    switch (filter) {
        case 0:
            return scanline;
        case 1:
            return UnfilterSub(scanline, width, bpp);
        case 2:
            return UnfilterUp(scanline, prevScanline, width, bpp);
        case 3:
            return UnfilterAverage(scanline, prevScanline, width, bpp);
        case 4:
            return UnfilterPaeth(scanline, prevScanline, width, bpp);
        default:
            throw std::invalid_argument("Invalid PNG");
    }
}

std::vector<uint8_t> PngReader::UnfilterSub(const std::vector<uint8_t>& scanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    for (int i = 0; i < width * bpp; i++) {
        if (i < bpp) {
            result.push_back(scanline[i]);
        } else {
            result.push_back(scanline[i] + result[i - bpp]);
        }
    }
    return result;
}

std::vector<uint8_t> PngReader::UnfilterUp(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    if (prevScanline.empty()) {
        return scanline;
    }

    for (int i = 0; i < width * bpp; i++) {
        result.push_back(scanline[i] + prevScanline[i]);
    }
    return result;
}

std::vector<uint8_t> PngReader::UnfilterAverage(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    for (int i = 0; i < width * bpp; i++) {
        if (i < bpp) {
            result.push_back(scanline[i] + prevScanline[i] / 2);
        } else {
            result.push_back(scanline[i] + floorf((result[i - bpp] + prevScanline[i]) / 2.0f));
        }
    }
    return result;
}

std::vector<uint8_t> PngReader::UnfilterPaeth(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    for (int i = 0; i < width * bpp; i++) {
        if (i < bpp) {
            result.push_back(scanline[i] + prevScanline[i]);
        } else {
            result.push_back(scanline[i] + PaethPredictor(result[i - bpp], prevScanline[i], prevScanline[i - bpp]));
        }
    }
    return result;
}

uint8_t PngReader::PaethPredictor(uint8_t a, uint8_t b, uint8_t c)
{
    int p = a + b - c;
    int pa = abs(p - a);
    int pb = abs(p - b);
    int pc = abs(p - c);
    if (pa <= pb && pa <= pc) {
        return a;
    } else if (pb <= pc) {
        return b;
    } else {
        return c;
    }
}

std::vector<std::vector<uint8_t>> PngReader::DecodeScanlines(const std::vector<uint8_t>& rawImg, uint8_t bpp, uint32_t width, uint32_t height)
{
    std::vector<std::vector<uint8_t>> scanlines;
    std::vector<uint8_t> prevScanline;

    for (int i = 0; i < height; i++) {
        std::vector<uint8_t> scanline;
        size_t filter = rawImg[i * (width * bpp + 1)];
        scanline.insert(scanline.end(), rawImg.begin() + i * (width * bpp + 1) + 1, rawImg.begin() + i * (width * bpp + 1) + 1 + width * bpp);
        scanline = Unfilter(scanline, prevScanline, filter, width, bpp);
        prevScanline = scanline;
        scanlines.push_back(scanline);
    }

    return scanlines;
}

bool PngHeader::Validate()
{
    if (compressionMethod != 0 || filterMethod != 0) {
        return false;
    }

    if (interlaceMethod != 0 && interlaceMethod != 1) {
        return false;
    }

    if (width == 0 || height == 0) {
        return false;
    }

    switch (colorType) {
        case 0:
            if (bitDepth != 1 && bitDepth != 2 && bitDepth != 4 && bitDepth != 8 && bitDepth != 16) {
                return false;
            }
            break;
        case 2:
            if (bitDepth != 8 && bitDepth != 16) {
                return false;
            }
            break;
        case 3:
            if (bitDepth != 1 && bitDepth != 2 && bitDepth != 4 && bitDepth != 8) {
                return false;
            }
            break;
        case 4:
            if (bitDepth != 8 && bitDepth != 16) {
                return false;
            }
            break;
        case 6:
            if (bitDepth != 8 && bitDepth != 16) {
                return false;
            }
            break;
        default:
            return false;
    }
    return true;
}