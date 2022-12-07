#include <memory>
#include <stdexcept>
#include "PngReader.h"
#include <vector>
#include <algorithm>
#include "zlib.h"
#include <cmath>

#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        std::cerr << msg << " error: " << err << std::endl; \
        exit(1); \
    } \
}

std::unique_ptr<Image> PngReader::ReadImage(const std::vector<uint8_t> &data) {
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

    std::vector<Chunk> idatChunks;
    for (int i = 1; i < chunks.size() - 1; i++) {
        if (chunks[i].type == 0x49444154) {
            idatChunks.push_back(chunks[i]);
        }
    }


    if (idatChunks.empty()) {
        throw std::invalid_argument("Invalid PNG");
    }

    return DecodeImage(idatChunks, header, palette);
}

bool PngReader::IsPngHeader(const std::vector<uint8_t> &data) {
    if (data.size() <= 8) {
        return false;
    }

    return data[0] == 0x89
    && data[1] == 0x50
    && data[2] == 0x4E
    && data[3] == 0x47
    && data[4] == 0x0D
    && data[5] == 0x0A
    && data[6] == 0x1A
    && data[7] == 0x0A;
}

std::vector<Chunk> PngReader::ReadChunks(const std::vector<uint8_t> &data) {
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

std::unique_ptr<Image> PngReader::DecodeImage(const std::vector<Chunk> &chunks, const PngHeader &header,
                                              const std::vector<plteChunk> &palette) {
    std::unique_ptr<Image> image = nullptr;

    switch (header.colorType) {
        case 0:
            image = DecodeGrayscaleImage(chunks, header);
            break;
        case 2:
            image = DecodeRGBImage(chunks, header, palette);
            break;
        case 3:
            image = DecodePaletteImage(chunks, header, palette);
            break;
        case 4:
            image = DecodeGrayscaleAlphaImage(chunks, header);
            break;
        case 6:
            image = DecodeRGBAImage(chunks, header, palette);
            break;
        default:
            throw std::invalid_argument("Invalid PNG");
    }

    return image;
}

std::unique_ptr<Image> PngReader::DecodeGrayscaleImage(const std::vector<Chunk> &chunks, const PngHeader &header) {
    throw std::invalid_argument("Not implemented");
    //TODO: Implement
}

std::unique_ptr<Image>
PngReader::DecodeRGBImage(const std::vector<Chunk> &chunks, const PngHeader &header, const std::vector<plteChunk> &palette) {
    std::vector<uint8_t> data;
    for(Chunk chunk : chunks)
    {
        data.insert(data.end(), chunk.data.begin(), chunk.data.end());
    }

    data = Inflate(data);

    std::vector<std::vector<uint8_t>> scanlines;

    std::vector<uint8_t> prevScanline;

    for(int i = 0; i < header.height; i++)
    {
        std::vector<uint8_t> scanline;
        size_t filter = data[i * (header.width * 3 + 1)];
        scanline.insert(scanline.end(), data.begin() + i * (header.width * 3 + 1) + 1, data.begin() + i * (header.width * 3 + 1) + 1 + header.width * 3);
        scanline = Unfilter(scanline, prevScanline, filter, header.width, 3);
        prevScanline = scanline;
        scanlines.push_back(scanline);
    }

    std::vector<Pixel> pixels;

    for (int i = 0; i < header.height; i++) {
        for (int j = 0; j < header.width; ++j) {
            uint8_t r = scanlines[i][j * 3];
            uint8_t g = scanlines[i][j * 3 + 1];
            uint8_t b = scanlines[i][j * 3 + 2];
            Pixel pixel = Pixel(r, g, b, 255);
            pixels.push_back(pixel);
        }
    }
    return std::make_unique<Image>(header.width, header.height, pixels);
}

std::unique_ptr<Image>
PngReader::DecodePaletteImage(const std::vector<Chunk> &chunks, const PngHeader &header, const std::vector<plteChunk> &palette) {
    throw std::invalid_argument("Not implemented");
    //TODO: implement
}

std::unique_ptr<Image> PngReader::DecodeGrayscaleAlphaImage(const std::vector<Chunk> &chunks, const PngHeader &header) {
    throw std::invalid_argument("Not implemented");
    //не нужно реализовывать
}

std::unique_ptr<Image>
PngReader::DecodeRGBAImage(const std::vector<Chunk> &chunks, const PngHeader &header, const std::vector<plteChunk> &palette) {
    throw std::invalid_argument("Not implemented");
    //не нужно реализовывать
}

std::vector<uint8_t> PngReader::Inflate(const std::vector<uint8_t> &data) {
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
    stream.next_in = const_cast<uint8_t *>(data.data());

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
    }
}

std::vector<uint8_t> PngReader::UnfilterSub(const std::vector<uint8_t>& scanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    for(int i = 0; i < width * bpp; i++)
    {
        if(i < bpp)
        {
            result.push_back(scanline[i]);
        }
        else
        {
            result.push_back(scanline[i] + result[i - bpp]);
        }
    }
    return result;
}

std::vector<uint8_t> PngReader::UnfilterUp(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    for(int i = 0; i < width * bpp; i++)
    {
        result.push_back(scanline[i] + prevScanline[i]);
    }
    return result;
}

std::vector<uint8_t> PngReader::UnfilterAverage(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    for(int i = 0; i < width * bpp; i++)
    {
        if(i < bpp)
        {
            result.push_back(scanline[i] + prevScanline[i] / 2);
        }
        else
        {
            result.push_back(scanline[i] + (result[i - bpp] + prevScanline[i]) / 2);
        }
    }
    return result;
}

std::vector<uint8_t> PngReader::UnfilterPaeth(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp)
{
    std::vector<uint8_t> result;
    for(int i = 0; i < width * bpp; i++)
    {
        if(i < bpp)
        {
            result.push_back(scanline[i] + prevScanline[i]);
        }
        else
        {
            int a = result[i - bpp];
            int b = prevScanline[i];
            int c = prevScanline[i - bpp];
            int p = a + b - c;
            int pa = abs(p - a);
            int pb = abs(p - b);
            int pc = abs(p - c);
            if(pa <= pb && pa <= pc)
            {
                result.push_back(scanline[i] + a);
            }
            else if(pb <= pc)
            {
                result.push_back(scanline[i] + b);
            }
            else
            {
                result.push_back(scanline[i] + c);
            }
        }
    }
    return result;
}

bool PngHeader::Validate() {
    if (compressionMethod != 0 || filterMethod != 0) {
        return false;
    }

    if (interlaceMethod != 0 && interlaceMethod != 1) {
        return false;
    }

    if (width == 0 || height == 0) {
        return false;
    }

    switch(colorType)
    {
        case 0:
            if(bitDepth != 1 && bitDepth != 2 && bitDepth != 4 && bitDepth != 8 && bitDepth != 16)
            {
                return false;
            }
            break;
        case 2:
            if(bitDepth != 8 && bitDepth != 16)
            {
                return false;
            }
            break;
        case 3:
            if(bitDepth != 1 && bitDepth != 2 && bitDepth != 4 && bitDepth != 8)
            {
                return false;
            }
            break;
        case 4:
            if(bitDepth != 8 && bitDepth != 16)
            {
                return false;
            }
            break;
        case 6:
            if(bitDepth != 8 && bitDepth != 16)
            {
                return false;
            }
            break;
        default:
            return false;
    }
    return true;
}
