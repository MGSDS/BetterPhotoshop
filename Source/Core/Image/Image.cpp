#include "Image.hpp"
#include "Core/Log.hpp"
#include "Core/Image/Reader/FormatReaderResolver.hpp"
#include "Core/Utils/Utils.hpp"
#include <fstream>
#include <exception>

size_t Image::GetWidth() const
{
    return m_Width;
}

size_t Image::GetHeight() const
{
    return m_Height;
}

size_t Image::GetPixelsCount() const
{
    return m_Size;
}

Pixel& Image::PixelAt(size_t row, size_t col)
{
    if (row >= m_Height) {
        Log::Error("Given 'row' value {} is out of range [0, {}).", row, m_Height);
        throw std::out_of_range("Given 'row' value is out of range.");
    }
    if (col >= m_Width) {
        Log::Error("Given 'col' value ({}) is out of range [0, {})", row, m_Width);
        throw std::out_of_range("Given 'col' value is out of range.");
    }
    return m_pixels[m_Width * row + col];
}

const Pixel& Image::PixelAt(size_t row, size_t col) const
{
    if (row >= m_Height) {
        Log::Error("Given 'row' value {} is out of range [0, {}).", row, m_Height);
        throw std::out_of_range("Given 'row' value is out of range.");
    }
    if (col >= m_Width) {
        Log::Error("Given 'col' value ({}) is out of range [0, {})", row, m_Width);
        throw std::out_of_range("Given 'col' value is out of range.");
    }
    return m_pixels[m_Width * row + col];
}

Pixel& Image::PixelAt(size_t index)
{
    if (index >= m_Width * m_Height) {
        Log::Error("Given 'index' value ({}) is out of range [0, {})", index, m_Size);
        throw std::out_of_range("Given 'index' value is out of range.");
    }
    return m_pixels[index];
}

const Pixel& Image::PixelAt(size_t index) const
{
    if (index >= m_Width * m_Height) {
        Log::Error("Given 'index' value ({}) is out of range [0, {})", index, m_Size);
        throw std::out_of_range("Given 'index' value is out of range.");
    }
    return m_pixels[index];
}

std::shared_ptr<Image> Image::FromFile(const std::string& fileName)
{
    std::ifstream in(fileName, std::ios::binary | std::ios::ate);

    if (!in.good()) {
        Log::Error("File {} does not exists.", fileName);
        throw std::runtime_error("File not found.");
    }

    std::vector<uint8_t> data = Utils::ReadAllBytes(in);
    in.close();

    Log::Info("{}: {} bytes read.", fileName, data.size());

    auto reader = FormatReaderResolver::GetReader(data);
    return reader->ReadImage(data);
}

Image::Image(size_t width, size_t height, const std::vector<Pixel>& pixels)
: m_Width(width), m_Height(height), m_Size(width * height)
{
    m_pixels = std::vector<Pixel>(pixels);
}

uint8_t* Image::ToDataARGB32() {
    return reinterpret_cast<uint8_t*>(&m_pixels[0]);
}

Image::Image(size_t width, size_t height)
: m_Width(width), m_Height(height), m_Size(width * height)
{
    m_pixels = std::vector<Pixel>(
            m_Size,
            Pixel {
                .blue = 255U,
                .green = 255U,
                .red = 255U,
                .alpha = 255U,
            });
}
