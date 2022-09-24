#pragma once

#include "Core/Image/Writer/ImageWriter.hpp"


class PpmWriter : public ImageWriter
{
public:
    void Write(const Image& image, std::ostream& os) const override;

    ~PpmWriter() override = default;

private:
    static void WriteHeader(const Image&, std::ostream& os);
};
