#include "Histogram.hpp"

#include "Core/Utils/Utils.hpp"

std::vector<std::vector<int>> Histogram::Calculate(const Image& image)
{
    std::vector<std::vector<int>> histogram(4, std::vector<int>(256, 0));

    for (int i = 0; i < image.GetPixelsCount(); ++i) {
        for (int j = 0; j < 3; ++j) {
            uint8_t normalizedValue = Utils::ByteFromNorm(image.PixelAt(i).channels[j]);
            ++histogram[j][normalizedValue];
        }
        float gray = image.PixelAt(i).channels[0] * 0.299f + image.PixelAt(i).channels[1] * 0.587f + image.PixelAt(i).channels[2] * 0.114f;
        uint8_t grayByte = Utils::ByteFromNorm(gray);
        ++histogram[3][grayByte];
    }

    return histogram;
}

std::unique_ptr<Image> Histogram::Correct(Image& image, const std::vector<std::vector<int>>& histogram)
{
    return std::unique_ptr<Image>();
}
