#include "YCbCr601Converter.hpp"

void YCbCr601Converter::ConvertPixelToRGB(Pixel &pixel) {
    float Y = pixel.channels[0];
    float Cb = pixel.channels[1];
    float Cr = pixel.channels[2];

    pixel.channels[0] = (Y - 16.0f) / 219.0f + 1.402f * (Cr - 128.0f) / 224;
    pixel.channels[1] = (Y - 16) / 219 - 1.772f * 0.114f / 0.587f * (Cb - 128.0f) / 224 - 1.402f * 0.299f / 0.587f * (Cr - 128.0f) / 224;
    pixel.channels[2] = (Y - 16) / 219 + 1.772f * (Cb - 128.0f) / 224;

}

void YCbCr601Converter::ConvertPixelFromRGB(Pixel &pixel) {
    float R = pixel.channels[0];
    float G = pixel.channels[1];
    float B = pixel.channels[2];

    float Y = 16.0f + 65.481f * R + 128.533f * G + 24.966f * B;
    float Cb = 128.0f - 37.797f * R + 74.203f * G + 112.0f * B;
    float Cr = 128.0f + 112.0f * R + 93.786f * G + 18.214f * B;

    pixel.channels[0] = Y;
    pixel.channels[1] = Cb;
    pixel.channels[2] = Cr;
}
