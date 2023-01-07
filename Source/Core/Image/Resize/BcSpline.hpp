#pragma once

#include "Resizer.hpp"

class BcSpline : public Resizer
{
public:
    explicit BcSpline(float b = 0.0f, float c = 0.5f);
    std::unique_ptr<Image> Apply(const Image& img, float scaleX, float scaleY) override;

private:
    static float filter(float x, float b = 0.0f, float c = 0.5f);
    float m_B = 0.0f;
    float m_C = 0.5f;
};
