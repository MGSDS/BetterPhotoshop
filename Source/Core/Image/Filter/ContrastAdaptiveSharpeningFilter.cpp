#include "ContrastAdaptiveSharpeningFilter.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <Core/Log.hpp>

ContrastAdaptiveSharpeningFilter::ContrastAdaptiveSharpeningFilter(float sharpness)
    : m_Sharpness(sharpness)
{
}

static std::pair<int, int> At(int x, int y, int maxW, int maxH)
{
    return std::make_pair(std::clamp(x, 0, maxW - 1), std::clamp(y, 0, maxH - 1));
}

std::unique_ptr<Image> ContrastAdaptiveSharpeningFilter::Apply(const Image& image)
{
    int width = image.GetWidth();
    int height = image.GetHeight();
    std::unique_ptr<Image> newImage = std::make_unique<Image>(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const Pixel& a = image.PixelAt(std::clamp<int>(y - 1, 0, height - 1), std::clamp<int>(x - 1, 0, width - 1));
            const Pixel& b = image.PixelAt(std::clamp<int>(y - 1, 0, height - 1), x);
            const Pixel& c = image.PixelAt(std::clamp<int>(y - 1, 0, height - 1), std::clamp<int>(x + 1, 0, width - 1));
            const Pixel& d = image.PixelAt(y, std::clamp<int>(x - 1, 0, width - 1));
            const Pixel& e = image.PixelAt(y, x);
            const Pixel& f = image.PixelAt(y, std::clamp<int>(x + 1, 0, width - 1));
            const Pixel& g = image.PixelAt(std::clamp<int>(y + 1, 0, height - 1), std::clamp<int>(x - 1, 0, width - 1));
            const Pixel& h = image.PixelAt(std::clamp<int>(y + 1, 0, height - 1), x);
            const Pixel& i = image.PixelAt(std::clamp<int>(y + 1, 0, height - 1), std::clamp<int>(x + 1, 0, width - 1));

            // Use all channels for weights, not only one
            std::vector<float> mn(3), mx(3);
            for (int ind = 0; ind < 3; ind++) {
                auto [minValue, maxValue] = std::minmax({ h.channels[ind], d.channels[ind], e.channels[ind], f.channels[ind], b.channels[ind] });
                // Better diagonals, also look at corner pixels
                minValue += std::min({ minValue, a.channels[ind], c.channels[ind], g.channels[ind], i.channels[ind] });
                maxValue += std::max({ maxValue, a.channels[ind], c.channels[ind], g.channels[ind], i.channels[ind] });
                mn[ind] = minValue;
                mx[ind] = maxValue;
            }

            std::vector<float> w(3);
            for (int ind = 0; ind < 3; ind++) {
                float value = std::sqrt(std::clamp(std::min(2.0f - mx[ind], mn[ind]) / mx[ind], 0.0f, 1.0f));
                w[ind] = value * (-0.125f - 0.075f * m_Sharpness);
            }

            Pixel& newPixel = newImage->PixelAt(y, x);
            for (int ind = 0; ind < 3; ind++) {
                float weight = 4.0f * w[ind] + 1.0f;
                float res = (w[ind] * (h.channels[ind] + d.channels[ind] + f.channels[ind] + b.channels[ind])) + e.channels[ind];
                newPixel.channels[ind] = std::clamp(res / weight, 0.0f, 1.0f);
            }
        }
    }

    return newImage;
}
