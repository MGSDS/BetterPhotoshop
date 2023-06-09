#pragma once

#include "ImageWriter.hpp"

class PgmWriter : public ImageWriter
{
public:
    void Write(const Image& image, std::ostream& os, uint8_t bitsPerChannel = 8, bool grayscale = false) const override;

    ~PgmWriter() override = default;

private:
    static void WriteHeader(const Image&, std::ostream& os);
};
