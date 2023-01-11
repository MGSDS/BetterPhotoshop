#include "BcSpline.hpp"

std::unique_ptr<Image> BcSpline::Apply(const Image& img, float scaleX, float scaleY)
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
                for (int i = -2; i <= 2; ++i) {
                    for (int j = -2; j <= 2; ++j) {
                        int srcX0 = static_cast<int>(srcX + i);
                        int srcY0 = static_cast<int>(srcY + j);
                        if (srcX0 < 0 || srcX0 >= img.GetWidth() || srcY0 < 0 || srcY0 >= img.GetHeight()) {
                            continue;
                        }

                        double weight = filter(i - srcX + srcX0, m_B, m_C) * filter(j - srcY + srcY0, m_B, m_C);
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

float BcSpline::filter(float x, float b, float c)
{
    float res = 0;
    x = std::abs(x);
    if (x < 1) {
        res = (12 - 9 * b - 6 * c) * x * x * x + (-18 + 12 * b + 6 * c) * x * x + (6 - 2 * b);
    } else if (x < 2) {
        res = (-b - 6 * c) * x * x * x + (6 * b + 30 * c) * x * x + (-12 * b - 48 * c) * x + (8 * b + 24 * c);
    }
    return res;
}

BcSpline::BcSpline(float b, float c)
    : m_B(b)
    , m_C(c)
{
}
