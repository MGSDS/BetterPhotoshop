#include "SimpleTresholdFilter.hpp"
#include <Core/Log.hpp>

#include <memory>
#include <stdexcept>

SimpleTresholdFilter::SimpleTresholdFilter(float treshold)
{
    if (treshold < 0.0f || treshold > 1.0f) {
        Log::Error("SimpleTresholdFilter: expected treshold between 0.0 and 1.0 but was {}", treshold);
        throw new std::runtime_error("Given treshold is out of bounds [0.0, 1.0]");
    }
    m_Treshold = treshold;
}

std::unique_ptr<Image> SimpleTresholdFilter::Apply(const Image& image)
{
    std::unique_ptr<Image> newImage = std::make_unique<Image>(image);

    for (size_t i = 0; i < newImage->GetPixelsCount(); i++) {
        Pixel& pixel = newImage->PixelAt(i);
        for (size_t j = 0; j < 3; j++) {
            pixel.channels[j] = (pixel.channels[j] < m_Treshold) ? 0.0f : 1.0f;
        }
    }

    return newImage;
}