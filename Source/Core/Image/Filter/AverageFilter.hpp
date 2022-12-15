#pragma once

#include "Filter.hpp"

class AverageFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    AverageFilter(size_t radius);
    virtual ~AverageFilter() override = default;

private:
    size_t m_Radius;
};