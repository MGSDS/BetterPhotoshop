#pragma once

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

    static std::shared_ptr<Image> FromFile(const std::string& fileName);
    virtual void WriteToFile(const std::string&) const = 0;

    uint8_t* ToDataARGB32();
    size_t GetWidth() const;
    size_t GetHeight() const;
    size_t GetPixelsCount() const;
    Pixel& PixelAt(size_t row, size_t col);
    const Pixel& PixelAt(size_t row, size_t col) const;
    Pixel& PixelAt(size_t index);
    const Pixel& PixelAt(size_t index) const;

    //FIXME: remove, this should be in writer
    virtual std::string GetExtension() const = 0;

protected:
    Image(size_t, size_t, const std::vector<Pixel>& pixels);
    virtual ~Image() = default;

private:
    Image() = default;
    Image(const Image&) = delete;

    size_t m_Width = 0;
    size_t m_Height = 0;
    size_t m_Size = 0;
    std::vector<Pixel> m_Pixels;
};
