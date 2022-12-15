#include "SobelFilter.hpp"
#include <algorithm>
#include "cmath"

std::unique_ptr<Image> SobelFilter::Apply(const Image& image)
{
    std::unique_ptr<Image> newImage = std::make_unique<Image>(image.GetWidth(), image.GetHeight());

    float kernelX[3][3] = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };

    float kernelY[3][3] = {
        { -1, -2, -1 },
        { 0, 0, 0 },
        { 1, 2, 1 }
    };

    for (int i = 0; i < image.GetHeight(); ++i)
    {
        for (int j = 0; j < image.GetWidth(); ++j)
        {
           Pixel& newPixel = newImage->PixelAt(i, j);

           float magX = 0.0f;
           for (int ch = 0; ch < 3; ++ch)
           {
               float pixelX = (kernelX[0][0] * GetPixel(image, i - 1, j - 1).channels[ch]) +
                               (kernelX[0][1] * GetPixel(image, i - 1, j).channels[ch]) +
                               (kernelX[0][2] * GetPixel(image, i - 1, j + 1).channels[ch]) +
                               (kernelX[1][0] * GetPixel(image, i, j - 1).channels[ch]) +
                               (kernelX[1][1] * GetPixel(image, i, j).channels[ch]) +
                               (kernelX[1][2] * GetPixel(image, i, j + 1).channels[ch]) +
                               (kernelX[2][0] * GetPixel(image, i + 1, j - 1).channels[ch]) +
                               (kernelX[2][1] * GetPixel(image, i + 1, j).channels[ch]) +
                               (kernelX[2][2] * GetPixel(image, i + 1, j + 1).channels[ch]);
               float pixelY = (kernelY[0][0] * GetPixel(image, i - 1, j - 1).channels[ch]) +
                               (kernelY[0][1] * GetPixel(image, i - 1, j).channels[ch]) +
                               (kernelY[0][2] * GetPixel(image, i - 1, j + 1).channels[ch]) +
                               (kernelY[1][0] * GetPixel(image, i, j - 1).channels[ch]) +
                               (kernelY[1][1] * GetPixel(image, i, j).channels[ch]) +
                               (kernelY[1][2] * GetPixel(image, i, j + 1).channels[ch]) +
                               (kernelY[2][0] * GetPixel(image, i + 1, j - 1).channels[ch]) +
                               (kernelY[2][1] * GetPixel(image, i + 1, j).channels[ch]) +
                               (kernelY[2][2] * GetPixel(image, i + 1, j + 1).channels[ch]);

               float mag = std::sqrt((pixelX * pixelX) + (pixelY * pixelY));

               if (mag > 1.0f)
               {
                   mag = 1.0f;
               }
               if (mag < 0.0f)
               {
                   mag = 0.0f;
               }

                newPixel.channels[ch] = mag;
           }
        }
    }
    return newImage;
}

Pixel SobelFilter::GetPixel(const Image& image, int x, int y)
{
    x = std::clamp(x, 0, (int)image.GetHeight() - 1);
    y = std::clamp(y, 0, (int)image.GetWidth() - 1);
    return image.PixelAt(x, y);
}
