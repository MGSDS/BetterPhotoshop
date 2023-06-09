#pragma once

#include <Core/Image/ImageFormat.hpp>

#include <cstdint>
#include <memory>
#include <vector>

struct Pixel
{
    float channels[4];

    Pixel(float ch0, float ch1, float ch2, float ch3);
    static Pixel WhiteRgb();
};

class Image;

struct LoadedImageData
{
    std::unique_ptr<Image> LoadedImage;
    ImageFormat Format;
};

enum ActiveChannel
{
    ALL = -1,
    ZEROTH = 0,
    FIRST = 1,
    SECOND = 2,
    // THIRD  = 3 // temporary not supported
};

class Image
{
public:
    Image(size_t width, size_t height, bool isGrayscale = false);
    Image(size_t width, size_t height, const std::vector<Pixel>& pixels, bool isGrayscale = false);
    Image(size_t width, size_t height, Pixel pixel, bool isGrayscale = false);
    Image(const Image& other) = default;

    static LoadedImageData FromFile(const std::string& fileName);
    static Image CopyWithChannelMask(const Image& image, ActiveChannel activeChannel);
    static std::unique_ptr<Image> MonochromeGradient(size_t width, size_t height);

    void AddLayer(const Image& image);

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

    void AssignGamma(float gamma);
    float GetGamma() const;
    bool IsGrayscale() const;

private:
    Image() = default;

    size_t m_Width = 0;
    size_t m_Height = 0;
    size_t m_Size = 0;
    float m_Gamma = 1.0f;
    std::vector<Pixel> m_Pixels;
    bool m_IsGrayscale = false;
};
