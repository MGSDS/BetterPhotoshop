#pragma once

#include "ImageWriter.hpp"

class PgmWriter : public ImageWriter
{
public:
    void Write(const Image& image, std::ostream& os) const override;

    ~PgmWriter() override = default;

private:
    static void WriteHeader(const Image&, std::ostream& os);
};
