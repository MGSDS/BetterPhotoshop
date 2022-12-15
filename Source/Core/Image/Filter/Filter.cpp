#include "Filter.hpp"
#include "AverageFilter.hpp"
#include "GaussianFilter.hpp"
#include "MedianFilter.hpp"
#include "SimpleTresholdFilter.hpp"

std::unique_ptr<Filter> Filter::GetFilter(FilterAlgo filterAlgo, float parameter)
{
    switch (filterAlgo)
    {
    case FilterAlgo::AVERAGE:
        return std::make_unique<AverageFilter>(parameter);
    case FilterAlgo::GAUSSIAN:
        return std::make_unique<GaussianFilter>(parameter);
    case FilterAlgo::MEDIAN:
        return std::make_unique<MedianFilter>(parameter);
    case FilterAlgo::SIMPLETRESHOLD:
        return std::make_unique<SimpleTresholdFilter>(parameter);
    default:
        return nullptr;
    }
}

FilterParameter::FilterParameter(const std::string& name, float min, float max, float defaultValue) :
    Name(name),
    Min(min),
    Max(max),
    Value(defaultValue)
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
