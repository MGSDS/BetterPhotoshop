#pragma once

#include "Image.hpp"


class PpmImage : public Image
{
public:
    PpmImage(size_t width, size_t height);

    void WriteToFile(const std::string& fileName) const override;
    PpmImage(size_t, size_t, std::vector<Pixel>& data);
    std::string GetExtension() const override;

    ~PpmImage() override = default;
};
