#pragma once

#include "ImageReader.hpp"
#include <memory>

struct PgmHeader
{
    size_t dataOffset = 0; // size in bytes
    size_t width = 0;
    size_t height = 0;
    uint8_t maxGreyValue = 0;
};

class PgmReader : public ImageReader
{
public:
    std::shared_ptr<Image> ReadImage(const std::vector<uint8_t>& data) override;

    ~PgmReader() override = default;

private:
    static PgmHeader ReadHeader(const std::vector<uint8_t>& data);
    static void ThrowInvalidHeaderError(size_t pos);
    static void ThrowInvalidPgmFormatDataError();
};
