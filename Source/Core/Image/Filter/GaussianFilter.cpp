#include "GaussianFilter.hpp"

#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <numeric>
#include <vector>

GaussianFilter::GaussianFilter(float sigma)
: m_Sigma(sigma)
{
    // https://stackoverflow.com/questions/3149279/optimal-sigma-for-gaussian-filtering-of-an-image#comment109661968_62002971
    m_Radius = std::ceil(m_Sigma * 3);
}

static float gauss(float sigma, int x, int y) 
{
   float sigma2 = sigma * sigma;
   float doublePi = M_PI * 2;

   return std::exp(-((x * x + y * y) / (2 * sigma2))) / (M_PI * 2 * sigma2);
}

std::unique_ptr<Image> GaussianFilter::Apply(const Image& image)
{
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();
    std::unique_ptr<Image> newImage = std::make_unique<Image>(width, height);

    size_t diameter = 2 * m_Radius + 1;

    float kernel[diameter][diameter];
    float sum = 0.0f;
    for (int y = -m_Radius; y <= m_Radius; y++) {
        for (int x = -m_Radius; x <= m_Radius; x++) {
            size_t kernelY = y + m_Radius;
            size_t kernelX = x + m_Radius;
            kernel[kernelY][kernelX] = gauss(m_Sigma, x, y);
            sum += kernel[kernelY][kernelX];
        }
    }
    for (int y = -m_Radius; y <= m_Radius; y++) {
        for (int x = -m_Radius; x <= m_Radius; x++) {
            size_t kernelY = y + m_Radius;
            size_t kernelX = x + m_Radius;
            kernel[kernelY][kernelX] /= sum;
        }
    }

    
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
                    size_t kernelY = y + m_Radius;
                    size_t kernelX = x + m_Radius;

                    float kernelValue = kernel[kernelY][kernelX];
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