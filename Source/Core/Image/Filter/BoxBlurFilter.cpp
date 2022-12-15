#include "BoxBlurFilter.hpp"
#include <algorithm>
#include <memory>

BoxBlurFilter::BoxBlurFilter(float radius)
    : m_Radius(radius)
{
}

std::unique_ptr<Image> BoxBlurFilter::Apply(const Image& image)
{
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();

    std::unique_ptr<Image> newImage = std::make_unique<Image>(image.GetWidth(), image.GetHeight());
    size_t diameter = m_Radius * 2 + 1;
    for (int i = 0; i < image.GetHeight(); ++i)
    {
        for (int j = 0; j < image.GetWidth(); ++j)
        {
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            for(int x = 0; x < diameter; ++x)
            {
                for(int y = 0; y < diameter; ++y)
                {
                    size_t imgRow = std::clamp(i + y - static_cast<int>(m_Radius), 0, (int)height - 1);
                    size_t imgCol = std::clamp(j + x - static_cast<int>(m_Radius), 0, (int)width - 1);
                    const Pixel& pixel = image.PixelAt(imgRow, imgCol);
                    r += pixel.channels[0];
                    g += pixel.channels[1];
                    b += pixel.channels[2];
                }
            }
            Pixel& newPixel = newImage->PixelAt(i, j);
            newPixel.channels[0] = r / (diameter * diameter);
            newPixel.channels[1] = g / (diameter * diameter);
            newPixel.channels[2] = b / (diameter * diameter);
        }
    }

    return newImage;
}