#pragma once

#include <Core/Image/ImageFormat.hpp>

#include <cstdint>
#include <vector>
#include <memory>

struct Pixel {
    float red;
    float green;
    float blue;
    float alpha;

    Pixel(float red, float green, float blue, float alpha);
    static Pixel White();
};

class Image
{
public:
    Image(size_t width, size_t height);
    Image(size_t width, size_t height, const std::vector<Pixel>& pixels);

    static std::unique_ptr<Image> FromFile(const std::string& fileName);
    void WriteToFile(const std::string& fileName, ImageFormat format) const;

    uint8_t* ToDataRGBA32FPx4();
    const uint8_t* ToDataRGBA32FPx4() const;
    size_t GetWidth() const;
    size_t GetHeight() const;
    size_t GetPixelsCount() const;
    Pixel& PixelAt(size_t row, size_t col);
    const Pixel& PixelAt(size_t row, size_t col) const;
    Pixel& PixelAt(size_t index);
    const Pixel& PixelAt(size_t index) const;

private:
    Image() = default;
    Image(const Image&) = delete;

    size_t m_Width = 0;
    size_t m_Height = 0;
    size_t m_Size = 0;
    std::vector<Pixel> m_Pixels;
};
