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

    for (int i = 1; i < image.GetHeight() - 1; ++i)
    {
        for (int j = 1; j < image.GetWidth() - 1; ++j)
        {
           Pixel& newPixel = newImage->PixelAt(i, j);

           float magX = 0.0f;
           for (int ch = 0; ch < 3; ++ch)
           {
               float pixelX = (kernelX[0][0] * image.PixelAt(i - 1, j - 1).channels[ch]) +
                               (kernelX[0][1] * image.PixelAt(i - 1, j).channels[ch]) +
                               (kernelX[0][2] * image.PixelAt(i - 1, j + 1).channels[ch]) +
                               (kernelX[1][0] * image.PixelAt(i, j - 1).channels[ch]) +
                               (kernelX[1][1] * image.PixelAt(i, j).channels[ch]) +
                               (kernelX[1][2] * image.PixelAt(i, j + 1).channels[ch]) +
                               (kernelX[2][0] * image.PixelAt(i + 1, j - 1).channels[ch]) +
                               (kernelX[2][1] * image.PixelAt(i + 1, j).channels[ch]) +
                               (kernelX[2][2] * image.PixelAt(i + 1, j + 1).channels[ch]);
               float pixelY = (kernelY[0][0] * image.PixelAt(i - 1, j - 1).channels[ch]) +
                               (kernelY[0][1] * image.PixelAt(i - 1, j).channels[ch]) +
                               (kernelY[0][2] * image.PixelAt(i - 1, j + 1).channels[ch]) +
                               (kernelY[1][0] * image.PixelAt(i, j - 1).channels[ch]) +
                               (kernelY[1][1] * image.PixelAt(i, j).channels[ch]) +
                               (kernelY[1][2] * image.PixelAt(i, j + 1).channels[ch]) +
                               (kernelY[2][0] * image.PixelAt(i + 1, j - 1).channels[ch]) +
                               (kernelY[2][1] * image.PixelAt(i + 1, j).channels[ch]) +
                               (kernelY[2][2] * image.PixelAt(i + 1, j + 1).channels[ch]);

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