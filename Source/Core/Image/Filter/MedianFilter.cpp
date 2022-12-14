#include "MedianFilter.hpp"

#include <algorithm>
#include <stdexcept>
#include <memory>
#include <vector>

MedianFilter::MedianFilter(size_t radius)
: m_Radius(radius)
{
}

std::unique_ptr<Image> MedianFilter::Apply(const Image& image)
{
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();
    std::unique_ptr<Image> newImage = std::make_unique<Image>(width, height);

    size_t diameter = 2 * m_Radius + 1;
    std::vector<std::vector<float>> windowValues(3, std::vector<float>(diameter * diameter));
    size_t medianIdx = windowValues[0].size() / 2;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            for (size_t channel = 0; channel < 3; channel++) {
                windowValues[channel].clear();
            }

            for (int offsetH = -m_Radius; offsetH <= m_Radius; offsetH++) {
                for (int offsetW = -m_Radius; offsetW <= m_Radius; offsetW++) {
                    size_t y = i + offsetH;
                    size_t x = j + offsetW;
                    y = std::clamp(y, (size_t)0, height - 1);
                    x = std::clamp(x, (size_t)0, width - 1);

                    const Pixel& pixel = image.PixelAt(y, x);
                    for (int channel = 0; channel < 3; channel++) {
                        windowValues[channel].push_back(pixel.channels[channel]);
                    }
                }
            }

            Pixel& newPixel = newImage->PixelAt(i, j);
            for (int channel = 0; channel < 3; channel++) {
                std::nth_element(
                    windowValues[channel].begin(),
                    windowValues[channel].begin() + medianIdx,
                    windowValues[channel].end()
                );
                newPixel.channels[channel] = windowValues[channel][medianIdx];
            }
        }
    }

    return newImage;
}