#pragma once

#include "Filter.hpp"

class ContrastAdaptiveSharpeningFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    ContrastAdaptiveSharpeningFilter(float sharpness);
    virtual ~ContrastAdaptiveSharpeningFilter() override = default;

private:
    float m_Sharpness;
};