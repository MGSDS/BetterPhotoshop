#include "HslConverter.hpp"

#include <algorithm>
#include <memory>
#include <cmath>

const float NORM_SIXTY = (60.0f / 360.0f);

void HslConverter::ConvertPixelToRGB(Pixel& pixel)
{
    float H = pixel.channels[0];
    float S = pixel.channels[1];
    float L = pixel.channels[2];
    float C = (1.0f - abs(2 * L - 1.0f)) * S; // chroma

    float H1 = H / NORM_SIXTY;
    float X = C * (1.0f - abs(static_cast<float>(fmod(H1, 2.0)) - 1.0f)); // intermediate

    if (0.0f <= H1 && H1 < 1.0f) {
        pixel.channels[0] = C;
        pixel.channels[1] = X;
        pixel.channels[2] = 0.0f;
    } else if (1.0f <= H1 && H1 < 2.0f) {
        pixel.channels[0] = X;
        pixel.channels[1] = C;
        pixel.channels[2] = 0.0f;
    } else if (2.0f <= H1 && H1 < 3.0f) {
        pixel.channels[0] = 0.0f;
        pixel.channels[1] = C;
        pixel.channels[2] = X;
    } else if (3.0f <= H1 && H1 < 4.0f) {
        pixel.channels[0] = 0.0f;
        pixel.channels[1] = X;
        pixel.channels[2] = C;
    } else if (4.0f <= H1 && H1 < 5.0f) {
        pixel.channels[0] = X;
        pixel.channels[1] = 0.0f;
        pixel.channels[2] = C;
    } else if (5.0f <= H1 && H1 < 6.0f) {
        pixel.channels[0] = C;
        pixel.channels[1] = 0.0f;
        pixel.channels[2] = X;
    }

    float m = L - C / 2.0f;

    for (size_t i = 0; i < 3; i++) {
        pixel.channels[i] += m;
    }
}


void HslConverter::ConvertPixelFromRGB(Pixel& pixel)
{
    float R = pixel.channels[0];
    float G = pixel.channels[1];
    float B = pixel.channels[2];

    float minX = std::min({R, G, B});
    float maxX = std::max({R, G, B});

    float V = maxX;
    float C = maxX - minX;

    float L = (minX + maxX) / 2.0f;

    float H = NORM_SIXTY;
    if (C == 0.0f) {
        H = 0.0f;
    } else if (V == R) {
        H *= (0.0f + (G - B) / C);
    } else if (V == G) {
        H *= (2.0f + (B - R) / C);
    } else if (V == B) {
        H *= (4.0 + (R - G) / C);
    }

    float S = 0.0f;
    if (0.0f < L && L < 1.0f) {
        S = (V - L) / std::min(L, 1 - L);
    }

    pixel.channels[0] = H;
    pixel.channels[1] = S;
    pixel.channels[2] = L;
}