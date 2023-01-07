#include "Lanczos3Resizer.hpp"
#include <cmath>

std::unique_ptr<Image> Lanczos3Resizer::Apply(const Image& img, float scaleX, float scaleY)
{
    int newWidth = img.GetWidth() * scaleX;
    int newHeight = img.GetHeight() * scaleY;
    auto result = std::make_unique<Image>(newWidth, newHeight);

    for (int y = 0; y < result->GetHeight(); ++y) {
        for (int x = 0; x < result->GetWidth(); ++x) {
            float srcX = x / scaleX;
            float srcY = y / scaleY;
            for (int c = 0; c < 4; ++c) {
                float sum = 0;
                float weightSum = 0;
                for (int i = -3; i <= 3; ++i) {
                    for (int j = -3; j <= 3; ++j) {
                        int srcX0 = static_cast<int>(srcX + i);
                        int srcY0 = static_cast<int>(srcY + j);
                        if (srcX0 < 0 || srcX0 >= img.GetWidth() || srcY0 < 0 || srcY0 >= img.GetHeight()) {
                            continue;
                        }

                        double weight = LanczosKernel(i) * LanczosKernel(j);
                        sum += img.PixelAt(srcY0, srcX0).channels[c] * weight;
                        weightSum += weight;
                    }
                }
                result->PixelAt(y, x).channels[c] = sum / weightSum;
            }
        }
    }

    return result;
}

double Lanczos3Resizer::LanczosKernel(uint8_t x, uint8_t a)
{
    if (x == 0) {
        return 1;
    } else if (-a <= x && x < a) {
        return a * std::sin(M_PI * x) * std::sin(M_PI * x / a) / (M_PI * M_PI * x * x);
    } else {
        return 0;
    }
}
