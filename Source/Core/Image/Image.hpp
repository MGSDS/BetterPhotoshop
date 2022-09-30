#pragma once

#include <Core/Image/ImageFormat.hpp>

#include <cstdint>
#include <vector>
#include <memory>

struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
};

class Image
{
public:
    Image(size_t width, size_t height);
    Image(size_t width, size_t height, const std::vector<Pixel>& pixels);

    static std::shared_ptr<Image> FromFile(const std::string& fileName);
    void WriteToFile(const std::string& fileName, ImageFormat format) const;

    uint8_t* ToDataARGB32();
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