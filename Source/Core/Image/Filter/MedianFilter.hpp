#pragma once

#include "Filter.hpp"

class MedianFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    MedianFilter(size_t radius);
    virtual ~MedianFilter() override = default;

private:
    size_t m_Radius;
};