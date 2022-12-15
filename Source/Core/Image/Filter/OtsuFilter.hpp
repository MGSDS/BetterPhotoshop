#pragma once

#include "Filter.hpp"

class OtsuFilter : public Filter
{
public:
    std::unique_ptr<Image> Apply(const Image& image) override;
    OtsuFilter() = default;
    virtual ~OtsuFilter() override = default;
private:
    static std::vector<int> GetHistogram(const Image& image);
    static int GetThreshold(const std::vector<int>& histogram);
};