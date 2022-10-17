#include "YCbCr601Converter.hpp"

void YCbCr601Converter::ConvertPixelToRGB(Pixel &pixel) {
    float Y = pixel.channels[0] * 255.0f;
    float Cb = pixel.channels[1] * 255.0f;
    float Cr = pixel.channels[2] * 255.0f;

    float R = (Y - 16.0f) / 219.0f + 1.402f * (Cr - 128.0f) / 224;
    float G = (Y - 16) / 219 - 1.772f * 0.114f / 0.587f * (Cb - 128.0f) / 224 - 1.402f * 0.299f / 0.587f * (Cr - 128.0f) / 224;
    float B = (Y - 16) / 219 + 1.772f * (Cb - 128.0f) / 224;

    pixel.channels[0] = R;
    pixel.channels[1] = G;
    pixel.channels[2] = B;

}

void YCbCr601Converter::ConvertPixelFromRGB(Pixel &pixel) {
    float R = pixel.channels[0];
    float G = pixel.channels[1];
    float B = pixel.channels[2];

    float Y = 16.0f + 65.481f * R + 128.533f * G + 24.966f * B;
    float Cb = 128.0f - 37.797f * R - 74.203f * G + 112.0f * B;
    float Cr = 128.0f + 112.0f * R - 93.786f * G - 18.214f * B;

    pixel.channels[0] = Y / 255.0f;
    pixel.channels[1] = Cb / 255.0f;
    pixel.channels[2] = Cr / 255.0f;
}
