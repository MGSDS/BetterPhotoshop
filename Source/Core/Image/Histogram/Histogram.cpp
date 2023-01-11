#include "Histogram.hpp"

#include "Core/Utils/Utils.hpp"
#include <algorithm>

std::vector<std::vector<int>> Histogram::Calculate(const Image& image)
{
    std::vector<std::vector<int>> histogram(4, std::vector<int>(256, 0));

    for (int i = 0; i < image.GetPixelsCount(); ++i) {
        for (int j = 0; j < 3; ++j) {
            uint8_t normalizedValue = Utils::ByteFromNorm(image.PixelAt(i).channels[j]);
            ++histogram[j][normalizedValue];
        }
        float gray;
        if (image.IsGrayscale()) {
            gray = image.PixelAt(i).channels[0];
        } else {
            gray = image.PixelAt(i).channels[0] * 0.299f + image.PixelAt(i).channels[1] * 0.587f + image.PixelAt(i).channels[2] * 0.114f;
        }

        uint8_t grayByte = Utils::ByteFromNorm(gray);
        ++histogram[3][grayByte];
    }

    return histogram;
}

std::unique_ptr<Image> Histogram::Correct(Image& image, float ignore)
{
    std::vector<std::vector<int>> histogram = Calculate(image);
    auto newImage = std::make_unique<Image>(image.GetWidth(), image.GetHeight(), image.IsGrayscale());

    // find lowest and highest values with ignore

    int lowest = 0;
    int highest = 255;
    int ignorePixels = image.GetPixelsCount() * ignore;
    int currentPixels = 0;
    for (int i = 0; i < 256; ++i) {
        if (image.IsGrayscale()) {
            currentPixels += histogram[3][i];
        } else {
            currentPixels += histogram[0][i] + histogram[1][i] + histogram[2][i];
        }
        if (currentPixels > ignorePixels) {
            lowest = i;
            break;
        }
    }

    currentPixels = 0;
    for (int i = 255; i >= 0; --i) {
        if (image.IsGrayscale()) {
            currentPixels += histogram[3][i];
        } else {
            currentPixels += histogram[0][i] + histogram[1][i] + histogram[2][i];
        }
        if (currentPixels > ignorePixels) {
            highest = i;
            break;
        }
    }

    // clip values
    for (int i = 0; i < image.GetPixelsCount(); ++i) {
        for (int j = 0; j < 3; ++j) {
            uint8_t normalizedValue = Utils::ByteFromNorm(image.PixelAt(i).channels[j]);
            float newNormalizedValue = (float)(normalizedValue - lowest) / (float)(highest - lowest);
            newNormalizedValue = std::clamp(newNormalizedValue, 0.0f, 1.0f);
            newImage->PixelAt(i).channels[j] = newNormalizedValue;
        }
    }

    return newImage;
}
