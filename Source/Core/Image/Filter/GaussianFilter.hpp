#pragma once

#include "Filter.hpp"

class GaussianFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    GaussianFilter(float sigma);
    virtual ~GaussianFilter() override = default;

private:
    float m_Sigma;
    float m_Radius;
};