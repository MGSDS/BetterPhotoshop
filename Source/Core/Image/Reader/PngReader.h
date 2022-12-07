#pragma once

#include "ImageReader.hpp"

struct Chunk{
    uint32_t length;
    uint32_t type;
    std::vector<uint8_t> data;
    uint32_t crc;
};

struct PngHeader
{
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;

    bool Validate();
};

struct plteChunk
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class PngReader : public ImageReader
{
public:
    PngReader() = default;
    std::unique_ptr<Image> ReadImage(const std::vector<uint8_t>& data) override;

private:
    static bool IsPngHeader(const std::vector<uint8_t>& data);
    static std::vector<Chunk> ReadChunks(const std::vector<uint8_t>& data);

    static std::unique_ptr<Image> DecodeImage(const std::vector<uint8_t>& rawImg, const PngHeader& header,const std::vector<plteChunk>& palette);
    static std::unique_ptr<Image> DecodeGrayscaleImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader &header);
    static std::unique_ptr<Image> DecodeRGBImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader &header, const std::vector<plteChunk> &palette);
    static std::unique_ptr<Image> DecodePaletteImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader &header, const std::vector<plteChunk> &palette);
    static std::unique_ptr<Image> DecodeGrayscaleAlphaImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader &header);
    static std::unique_ptr<Image> DecodeRGBAImage(const std::vector<std::vector<uint8_t>>& scanlines, const PngHeader &header, const std::vector<plteChunk> &palette);

    static std::vector<std::vector<uint8_t>> DecodeScanlines(const std::vector<uint8_t>& rawImg,  uint8_t bpp, uint32_t width, uint32_t height);

    static std::vector<uint8_t> Inflate(const std::vector<uint8_t>& data);

    static std::vector<uint8_t> Unfilter(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint8_t filter, uint32_t width, uint8_t bpp);
    static std::vector<uint8_t> UnfilterSub(const std::vector<uint8_t>& scanline, uint32_t width, uint8_t bpp);
    static std::vector<uint8_t> UnfilterUp(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp);
    static std::vector<uint8_t> UnfilterAverage(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp);
    static std::vector<uint8_t> UnfilterPaeth(const std::vector<uint8_t>& scanline, const std::vector<uint8_t>& prevScanline, uint32_t width, uint8_t bpp);
    static uint8_t PaethPredictor(uint8_t a, uint8_t b, uint8_t c);
};