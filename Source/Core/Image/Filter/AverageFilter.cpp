#include "AverageFilter.hpp"

#include <algorithm>
#include <stdexcept>
#include <memory>
#include <numeric>
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

            for (int y = -m_Radius; y <= m_Radius; y++) {
                for (int x = -m_Radius; x <= m_Radius; x++) {
                    size_t imgRow = std::clamp(i + y, 0, (int)height - 1);
                    size_t imgCol = std::clamp(j + x, 0, (int)width - 1);

                    const Pixel& pixel = image.PixelAt(imgRow, imgCol);
                    for (size_t channel = 0; channel < 3; channel++) {
                        newPixel.channels[channel] += kernelValue * pixel.channels[channel];
                    }
                }
            }

            // сраное говно, значения всех каналов по прежнему нулевые

            for (size_t channel = 0; channel < 3; channel++) {
                newPixel.channels[channel] = std::clamp(newPixel.channels[channel], 0.0f, 1.0f);
            }


            Log::Debug("{} {} {}", newPixel.channels[0], newPixel.channels[1], newPixel.channels[2]);
        }
    }

    return newImage;
}