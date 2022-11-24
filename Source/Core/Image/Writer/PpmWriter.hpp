#pragma once

#include "Core/Image/Writer/ImageWriter.hpp"

class PpmWriter : public ImageWriter
{
public:
    void Write(const Image& image, std::ostream& os, uint8_t bitsPerChannel = 8) const override;

    ~PpmWriter() override = default;

private:
    static void WriteHeader(const Image&, std::ostream& os);
};
