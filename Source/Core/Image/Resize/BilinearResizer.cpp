#include "BilinearResizer.hpp"

std::unique_ptr<Image> BilinearResizer::Apply(const Image& img, float scaleX, float scaleY)
{
    int newWidth = img.GetWidth() * scaleX;
    int newHeight = img.GetHeight() * scaleY;
    auto result = std::make_unique<Image>(newWidth, newHeight);

    for (int y = 0; y < result->GetHeight(); ++y)
    {
        for (int x = 0; x < result->GetWidth(); ++x)
        {
            float srcX = x / scaleX;
            float srcY = y / scaleY;
            int srcX0 = static_cast<int>(srcX);
            int srcY0 = static_cast<int>(srcY);
            int srcX1 = std::clamp(srcX0 + 1, 0, static_cast<int>(img.GetWidth()) - 1);
            int srcY1 = std::clamp(srcY0 + 1, 0, static_cast<int>(img.GetHeight()) - 1);
            float dx = srcX - srcX0;
            float dy = srcY - srcY0;
            for (int c = 0; c < 4; ++c)
            {
                float p00 = img.PixelAt(srcY0, srcX0).channels[c];
                float p01 = img.PixelAt(srcY1, srcX0).channels[c];
                float p10 = img.PixelAt(srcY0, srcX1).channels[c];
                float p11 = img.PixelAt(srcY1, srcX1).channels[c];
                float p0 = p00 + (p10 - p00) * dx;
                float p1 = p01 + (p11 - p01) * dx;
                float p = p0 + (p1 - p0) * dy;
                result->PixelAt(y, x).channels[c] = p;
            }
        }
    }

    return result;
}
