#pragma once

#include "Image.hpp"


class PpmImage : public Image
{
public:
    void WriteToFile(const std::string& fileName) const override;
    PpmImage(size_t, size_t, std::vector<Pixel>& data);

    ~PpmImage() override = default;
};
