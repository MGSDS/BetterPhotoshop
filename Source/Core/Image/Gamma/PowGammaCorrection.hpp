#pragma once

#include <Core/Image/Image.hpp>

#include <cmath>

namespace Gamma {

    static inline void Correct(Image& image, float gammaValue)
    {
        for (size_t i = 0; i < image.GetPixelsCount(); i++) {
            auto& pixel = image.PixelAt(i);

            for (int j = 0; j < 3; j++) {
                pixel.channels[j] = std::pow(pixel.channels[j], 1.0f / gammaValue);
            }
        }
    }

} // namespace Gamma
