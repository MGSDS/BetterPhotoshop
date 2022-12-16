#include "AverageFilter.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <Core/Log.hpp>

AverageFilter::AverageFilter(size_t radius)
    : m_Radius(radius)
{
}

std::unique_ptr<Image> AverageFilter::Apply(const Image& image)
{
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();
    std::unique_ptr<Image> newImage = std::make_unique<Image>(width, height);

    size_t diameter = 2 * m_Radius + 1;
    float kernelValue = 1.0f / (diameter * diameter);

    Log::Debug("kernel value: {}", kernelValue);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel& newPixel = newImage->PixelAt(i, j);
            for (size_t channel = 0; channel < 3; channel++) {
                newPixel.channels[channel] = 0.0f;
            }

            for (int y = 0; y < diameter; y++) {
                for (int x = 0; x < diameter; x++) {
                    // BUG: cast size_t to int
                    size_t imgRow = std::clamp(i + y - static_cast<int>(m_Radius), 0, (int)height - 1);
                    size_t imgCol = std::clamp(j + x - static_cast<int>(m_Radius), 0, (int)width - 1);

                    const Pixel& pixel = image.PixelAt(imgRow, imgCol);
                    for (size_t channel = 0; channel < 3; channel++) {
                        newPixel.channels[channel] += kernelValue * pixel.channels[channel];
                    }
                }
            }

            for (size_t channel = 0; channel < 3; channel++) {
                newPixel.channels[channel] = std::clamp(newPixel.channels[channel], 0.0f, 1.0f);
            }
        }
    }

    return newImage;
}