#pragma once

#include <Core/Image/Image.hpp>

#include <algorithm>
#include <cmath>

namespace Gamma {

    static inline void CorrectToSrgb(Image& image)
    {
        for (size_t i = 0; i < image.GetPixelsCount(); i++) {
            auto& pixel = image.PixelAt(i);

            for (int j = 0; j < 3; j++) {
                float& channel = pixel.channels[j];

                if (channel <= 0.0031308f) {
                    channel *= 12.92f;
                } else {
                    channel = (1.055f * std::pow(channel, 1.0f / 2.4f)) - 0.055f;
                }

                channel = std::clamp(channel, 0.0f, 1.0f);
            }
        }
        image.SetGamma(0.0f);
    }

} // namespace Gamma
