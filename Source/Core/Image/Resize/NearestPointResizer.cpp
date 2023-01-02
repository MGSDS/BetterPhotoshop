#include "NearestPointResizer.hpp"

std::unique_ptr<Image> NearestPointResizer::Apply(const Image& img, float scaleX, float scaleY)
{
    int newWidth = img.GetWidth() * scaleX;
    int newHeight = img.GetHeight() * scaleY;
    auto result = std::make_unique<Image>(newWidth, newHeight);

    for (int y = 0; y < result->GetHeight(); ++y)
    {
        for (int x = 0; x < result->GetWidth(); ++x)
        {
            int srcX = x / scaleX;
            int srcY = y / scaleY;
            for (int c = 0; c < 4; ++c)
            {
                result->PixelAt(y, x).channels[c] = img.PixelAt(srcY, srcX).channels[c];
            }
        }
    }
    return result;
}