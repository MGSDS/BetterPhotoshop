#include "Painter.hpp"

Image Painter::DrawLine(size_t imWidth, size_t imHeight, float x0, float y0, float x1, float y1, int width, const Pixel& color)
{
    // Xiaolin Wu's line algorithm
    auto res = Image(imWidth, imHeight, Pixel(0.0f, 0.0f, 0.0f, 0.0f));

    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float dx = x1 - x0;

    float dy = y1 - y0;
    float gradient = 1.0f;
    if (dx != 0.0f) {
        gradient = dy / dx;
    }

    width *= static_cast<int>(sqrtf(1 + gradient * gradient));

    float xend = std::round(x0);
    float yend = y0 - (width - 1.0f) * 0.5f + gradient * (xend - x0);
    float xgap = 1.0f - std::fmod(x0 + 0.5f, 1.0f);
    int xpxl1 = static_cast<int>(xend);
    int ypxl1 = std::floor(yend);

    if (steep) {
        SetPixel(res, ypxl1, xpxl1, color, 1.0f - std::fmod(yend, 1.0f) * xgap);
        for (int i = 1; i < width; i++) {
            SetPixel(res, ypxl1 + i, xpxl1, color, 1.0f);
        }

        SetPixel(res, ypxl1 + width, xpxl1, color, std::fmod(yend, 1.0f) * xgap);
    } else {
        SetPixel(res, xpxl1, ypxl1, color, 1.0f - std::fmod(yend, 1.0f) * xgap);
        for (int i = 1; i < width; i++) {
            SetPixel(res, xpxl1, ypxl1 + i, color, 1.0f);
        }

        SetPixel(res, xpxl1, ypxl1 + width, color, std::fmod(yend, 1.0f) * xgap);
    }

    float intery = yend + gradient;

    xend = std::round(x1);
    yend = y1 + gradient * (xend - x1) - (width - 1.0f) * 0.5f;
    xgap = std::fmod(x1 + 0.5f, 1.0f);
    int xpxl2 = static_cast<int>(xend);
    int ypxl2 = std::floor(yend);

    if (steep) {
        SetPixel(res, ypxl2, xpxl2, color, 1.0f - std::fmod(yend, 1.0f) * xgap);
        for (int i = 1; i < width; i++) {
            SetPixel(res, ypxl2 + i, xpxl2, color, 1.0f);
        }
        SetPixel(res, ypxl2 + width, xpxl2, color, std::fmod(yend, 1.0f) * xgap);
    } else {
        SetPixel(res, xpxl2, ypxl2, color, 1.0f - std::fmod(yend, 1.0f) * xgap);
        for (int i = 1; i < width; i++) {
            SetPixel(res, xpxl2, ypxl2 + i, color, 1.0f);
        }

        SetPixel(res, xpxl2, ypxl2 + width, color, std::fmod(yend, 1.0f) * xgap);
    }

    if (steep) {
        for (int x = xpxl1 + 1; x < xpxl2; x++) {
            SetPixel(res, std::floor(intery), x, color, 1.0f - std::fmod(intery, 1.0f));
            for (int i = 1; i < width; i++) {
                SetPixel(res, std::floor(intery) + i, x, color, 1.0f);
            }

            SetPixel(res, std::floor(intery) + width, x, color, std::fmod(intery, 1.0f));
            intery += gradient;
        }
    } else {
        for (int x = xpxl1 + 1; x < xpxl2; x++) {
            SetPixel(res, x, std::floor(intery), color, 1.0f - std::fmod(intery, 1.0f));
            for (int i = 1; i < width; i++) {
                SetPixel(res, x, std::floor(intery) + i, color, 1.0f);
            }

            SetPixel(res, x, std::floor(intery) + width, color, std::fmod(intery, 1.0f));
            intery += gradient;
        }
    }

    return res;
}

void Painter::SetPixel(Image& image, int x, int y, Pixel color, float alpha)
{
    if (x < 0 || x >= image.GetWidth() || y < 0 || y >= image.GetHeight()) {
        return;
    }
    color.channels[3] *= alpha;
    image.PixelAt(y, x) = color;
}