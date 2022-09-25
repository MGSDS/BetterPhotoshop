#pragma once

#include "Image.hpp"


class PgmImage : public Image
{
public:
    PgmImage(size_t width, size_t height);

    void WriteToFile(const std::string&) const override;
    PgmImage(size_t width, size_t height, std::vector<Pixel>& data);
    std::string GetExtension() const override;

    ~PgmImage() override = default;
};

