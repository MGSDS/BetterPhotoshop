#include "YCbCrConverter.hpp"

void YCbCrConverter::ConvertPixelToRGB(Pixel& pixel)
{
    float Y = pixel.channels[0] * 255.0f;
    float Cb = pixel.channels[1] * 255.0f;
    float Cr = pixel.channels[2] * 255.0f;

    Y = (Y - 16.0f) / 219.0f;
    float Pb = (Cb - 128.0f) / 224.0f;
    float Pr = (Cr - 128.0f) / 224.0f;

    float R = 2.0f * Pr * (1 - Kr) + Y;
    float B = 2.0f * Pb * (1 - Kb) + Y;
    float G = (Y - Kr * R - Kb * B) / (1 - Kr - Kb);

    pixel.channels[0] = R;
    pixel.channels[1] = G;
    pixel.channels[2] = B;
}

void YCbCrConverter::ConvertPixelFromRGB(Pixel& pixel)
{
    float R = pixel.channels[0];
    float G = pixel.channels[1];
    float B = pixel.channels[2];

    float Y = Kr * R + (1 - Kr - Kb) * G + Kb * B;
    float Pb = 0.5f * (B - Y) / (1 - Kb);
    float Pr = 0.5f * (R - Y) / (1 - Kr);

    Y = 16.0f + 219.0f * Y;
    float Cb = 128.0f + 224.0f * Pb;
    float Cr = 128.0f + 224.0f * Pr;

    pixel.channels[0] = Y / 255.0f;
    pixel.channels[1] = Cb / 255.0f;
    pixel.channels[2] = Cr / 255.0f;
}

YCbCrConverter::YCbCrConverter(float Kb, float Kr)
    : Kb(Kb), Kr(Kr)
{
}
