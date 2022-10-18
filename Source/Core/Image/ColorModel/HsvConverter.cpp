#include "HsvConverter.hpp"
#include <algorithm>
#include <cmath>

const float NORM_SIXTY = (60.0f / 360.0f);

void HsvConverter::ConvertPixelFromRGB(Pixel& pixel)
{
    float R = pixel.channels[0];
    float G = pixel.channels[1];
    float B = pixel.channels[2];

    float min = std::min({ R, G, B });
    float max = std::max({ R, G, B });
    float delta = max - min;

    float H = 0.0f;

    if (delta == 0) {
    } else if (max == R && G >= B) {
        H = NORM_SIXTY * (G - B) / delta;
    } else if (max == R && G < B) {
        H = NORM_SIXTY * (G - B) / delta + 1;
    } else if (max == G) {
        H = NORM_SIXTY * (B - R) / delta + 1.0f / 3.0f;
    } else if (max == B) {
        H = NORM_SIXTY * (R - G) / delta + 2.0f / 3.0f;
    }

    float S = 0.0f;

    if (max != 0) {
        S = 1 - min / max;
    }

    float V = max;

    pixel.channels[0] = H;
    pixel.channels[1] = S;
    pixel.channels[2] = V;
}

void HsvConverter::ConvertPixelToRGB(Pixel& pixel)
{
    float H = pixel.channels[0];
    float S = pixel.channels[1];
    float V = pixel.channels[2];

    int Hi = static_cast<int>(std::floor(H * 6)) % 6;

    float Vmin = (1 - S) * V;

    float a = (V - Vmin) * std::fmod(H, 6.0f) / 6.0f;

    float Vinc = Vmin + a;
    float Vdec = V - a;

    float R = 0;
    float G = 0;
    float B = 0;

    switch (Hi) {
        case 0:
            R = V;
            G = Vinc;
            B = Vmin;
            break;
        case 1:
            R = Vdec;
            G = V;
            B = Vinc;
            break;
        case 2:
            R = Vmin;
            G = V;
            B = Vinc;
            break;
        case 3:
            R = Vmin;
            G = Vdec;
            B = V;
            break;
        case 4:
            R = Vinc;
            G = Vmin;
            B = Vdec;
            break;
        case 5:
            R = V;
            G = Vmin;
            B = Vdec;
            break;
    }

    pixel.channels[0] = R;
    pixel.channels[1] = G;
    pixel.channels[2] = B;
}
