#include "YCoCgConverter.hpp"

void YCoCgConverter::ConvertPixelFromRGB(Pixel& pixel)
{
    float R = pixel.channels[0];
    float G = pixel.channels[1];
    float B = pixel.channels[2];

    float Y = 0.25f * R + 0.5f * G + 0.25f * B;   // [0, 1]
    float Co = 0.5f * R - 0.5f * B;               // [-0.5, 0.5]
    float Cg = -0.25f * R + 0.5f * G - 0.25f * B; // [-0.5, 0.5]

    float normCo = Co + 0.5f; // [0, 1]
    float normCg = Cg + 0.5f; // [0, 1]

    pixel.channels[0] = Y;
    pixel.channels[1] = normCo;
    pixel.channels[2] = normCg;
}

void YCoCgConverter::ConvertPixelToRGB(Pixel& pixel)
{
    float Y = pixel.channels[0];
    float normCo = pixel.channels[1];
    float normCg = pixel.channels[2];

    float Co = normCo - 0.5f;
    float Cg = normCg - 0.5f;

    float R = Y + Co - Cg;
    float G = Y + Cg;
    float B = Y - Co - Cg;

    pixel.channels[0] = R;
    pixel.channels[1] = G;
    pixel.channels[2] = B;
}
