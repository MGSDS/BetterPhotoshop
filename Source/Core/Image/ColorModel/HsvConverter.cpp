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
    float C = max - min;

    float H = 0.0f;

    if (C == 0) {
    } else if (max == R && G >= B) {
        H = NORM_SIXTY * ((G - B) / C);
    } else if (max == R) {
        H = NORM_SIXTY * ((G - B) / C + 6.0f);
    } else if (max == G) {
        H = NORM_SIXTY * ((B - R) / C + 2.0f);
    } else if (max == B) {
        H = NORM_SIXTY * ((R - G) / C + 4.0f);
    }

    float S = 0.0f;

    float V = max;

    if (max != 0) {
        S = C / V;
    }

    pixel.channels[0] = H;
    pixel.channels[1] = S;
    pixel.channels[2] = V;
}

void HsvConverter::ConvertPixelToRGB(Pixel& pixel)
{
    float H = pixel.channels[0];
    float S = pixel.channels[1];
    float V = pixel.channels[2];

    float C = V * S;

    float Hi = H / NORM_SIXTY;

    float X = C * (1 - std::abs(std::fmod(Hi, 2.0f) - 1));

    float R = 0;
    float G = 0;
    float B = 0;

    if (0 <= Hi && Hi < 1) {
        R = C;
        G = X;
        B = 0;
    } else if (1 <= Hi && Hi < 2) {
        R = X;
        G = C;
        B = 0;
    } else if (2 <= Hi && Hi < 3) {
        R = 0;
        G = C;
        B = X;
    } else if (3 <= Hi && Hi < 4) {
        R = 0;
        G = X;
        B = C;
    } else if (4 <= Hi && Hi < 5) {
        R = X;
        G = 0;
        B = C;
    } else if (5 <= Hi && Hi < 6) {
        R = C;
        G = 0;
        B = X;
    }

    float m = V - C;

    pixel.channels[0] = R + m;
    pixel.channels[1] = G + m;
    pixel.channels[2] = B + m;
}
