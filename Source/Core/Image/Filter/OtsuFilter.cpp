#include "OtsuFilter.hpp"

std::unique_ptr<Image> OtsuFilter::Apply(const Image& image)
{
    std::unique_ptr<Image> newImage = std::make_unique<Image>(image.GetWidth(), image.GetHeight());

    std::vector<int> histogram = GetHistogram(image);

    int threshold = GetThreshold(histogram);

    for (int i = 0; i < image.GetHeight(); ++i)
    {
        for (int j = 0; j < image.GetWidth(); ++j)
        {
            Pixel& newPixel = newImage->PixelAt(i, j);
            Pixel oldPixel = image.PixelAt(i, j);

            float gray = (oldPixel.channels[0] + oldPixel.channels[1] + oldPixel.channels[2]) / 3.0f;

            if (gray * 255 > threshold)
            {
                newPixel.channels[0] = 1.0f;
                newPixel.channels[1] = 1.0f;
                newPixel.channels[2] = 1.0f;
            }
            else
            {
                newPixel.channels[0] = 0.0f;
                newPixel.channels[1] = 0.0f;
                newPixel.channels[2] = 0.0f;
            }
        }
    }

    return newImage;
}

std::vector<int> OtsuFilter::GetHistogram(const Image& image)
{
    std::vector<int> histogram(256, 0);
    for (int i = 0; i < image.GetHeight(); ++i)
    {
        for (int j = 0; j < image.GetWidth(); ++j)
        {
            const Pixel& pixel = image.PixelAt(i, j);
            int gray = (pixel.channels[0] + pixel.channels[1] + pixel.channels[2]) / 3 * 255;
            histogram[gray]++;
        }
    }
    return histogram;
}

int OtsuFilter::GetThreshold(const std::vector<int>& histogram)
{
    int intensitySum = 0;
    for (int i = 0; i < 256; ++i)
    {
        intensitySum += histogram[i];
    }

    int sum = 0;
    for (int i = 0; i < 256; ++i)
    {
        sum += i * histogram[i];
    }

    int sumB = 0;
    int wB = 0;
    int wF = 0;
    int max = 0;
    int threshold = 0;

    for (int i = 0; i < 256; ++i)
    {
        wF = intensitySum - wB;
        if (wB > 0 && wF > 0)
        {
            int mF = (sum - sumB) / wF;
            int diff = (int)(wB * wF * (sumB / wB - mF) * (sumB / wB - mF));
            if (diff > max)
            {
                max = diff;
                threshold = i;
            }
        }
        wB += histogram[i];
        sumB += i * histogram[i];
    }

    return threshold;
}
