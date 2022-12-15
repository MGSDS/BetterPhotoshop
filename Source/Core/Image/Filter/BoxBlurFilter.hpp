#pragma once

#include "Filter.hpp"

class BoxBlurFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    BoxBlurFilter(float radius);
    virtual ~BoxBlurFilter() override = default;
private:
    float m_Radius;
};