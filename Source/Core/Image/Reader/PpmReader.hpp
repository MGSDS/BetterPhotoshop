#pragma once

#include "ImageReader.hpp"

#include <memory>

struct PpmHeader
{
    size_t width = 0;
    size_t height = 0;
    uint8_t maxCh0Value = 0;
    uint8_t maxCh1Value = 0;
    uint8_t maxCh2Value = 0;
    size_t dataOffset = 0;
};

class PpmReader : public ImageReader
{
public:
    std::unique_ptr<Image> ReadImage(const std::vector<uint8_t>& data) override;

    ~PpmReader() override = default;

private:
    static PpmHeader ReadHeader(const std::vector<uint8_t>& data);
    static void ThrowInvalidHeaderError(size_t pos);
    static void ThrowInvalidPpmFormatDataError();
};
