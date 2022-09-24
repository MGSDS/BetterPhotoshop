#pragma once

#include "Image.hpp"


class PgmImage : public Image
{
public:
    void WriteToFile(const std::string&) const override;
    PgmImage(size_t width, size_t height, std::vector<Pixel>& data);

    ~PgmImage() override = default;
};

