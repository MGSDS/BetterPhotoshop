#include "Filter.hpp"
#include "AverageFilter.hpp"
#include "ContrastAdaptiveSharpeningFilter.hpp"
#include "GaussianFilter.hpp"
#include "MedianFilter.hpp"
#include "OtsuFilter.hpp"
#include "SimpleTresholdFilter.hpp"
#include "SobelFilter.hpp"

std::unique_ptr<Filter> Filter::GetFilter(FilterAlgo filterAlgo, float parameter)
{
    switch (filterAlgo) {
        case FilterAlgo::AVERAGE:
            return std::make_unique<AverageFilter>(parameter);
        case FilterAlgo::CONTRAST_ADAPTIVE_SHARPENING:
            return std::make_unique<ContrastAdaptiveSharpeningFilter>(parameter);
        case FilterAlgo::GAUSSIAN:
            return std::make_unique<GaussianFilter>(parameter);
        case FilterAlgo::MEDIAN:
            return std::make_unique<MedianFilter>(parameter);
        case FilterAlgo::SIMPLETRESHOLD:
            return std::make_unique<SimpleTresholdFilter>(parameter);
        case FilterAlgo::SOBEL:
            return std::make_unique<SobelFilter>();
        case FilterAlgo::OTSU:
            return std::make_unique<OtsuFilter>();
        default:
            return nullptr;
    }
}

FilterParameter::FilterParameter(const std::string& name, float min, float max, float defaultValue)
    : Name(name), Min(min), Max(max), Value(defaultValue)
{
}

float FilterParameter::GetValue() const
{
    return Value;
}

float FilterParameter::GetMax() const
{
    return Max;
}

float FilterParameter::GetMin() const
{
    return Min;
}

std::string FilterParameter::GetName() const
{
    return Name;
}
