#pragma once

#include "Filter.hpp"

class SimpleTresholdFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    SimpleTresholdFilter(float treshold);
    virtual ~SimpleTresholdFilter() override = default;

private:
    float m_Treshold;
};