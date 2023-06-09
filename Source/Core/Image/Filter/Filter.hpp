#pragma once

#include "Core/Image/Image.hpp"

enum FilterAlgo : uint8_t
{
    AVERAGE,
    CONTRAST_ADAPTIVE_SHARPENING,
    GAUSSIAN,
    MEDIAN,
    SIMPLETRESHOLD,
    SOBEL,
    OTSU
};

struct FilterParameter
{
public:
    FilterParameter(const std::string& name, float min, float max, float defaultValue);
    std::string GetName() const;
    float GetMin() const;
    float GetMax() const;
    float GetValue() const;

private:
    std::string Name;
    float Min;
    float Max;
    float Value;
};

static const std::unordered_map<FilterAlgo, std::string> ENUM_TO_STRING_FILTER_MAPPING = {
    { FilterAlgo::AVERAGE, "Average" },
    { FilterAlgo::CONTRAST_ADAPTIVE_SHARPENING, "CAS (AMD ©)" },
    { FilterAlgo::GAUSSIAN, "Gaussian" },
    { FilterAlgo::MEDIAN, "Median" },
    { FilterAlgo::SIMPLETRESHOLD, "Simple treshold" },
    { FilterAlgo::SOBEL, "Sobel" },
    { FilterAlgo::OTSU, "Otsu" }
};

static const std::unordered_map<FilterAlgo, std::shared_ptr<FilterParameter>> ENUM_TO_STRING_FILTER_PARAMETER_MAPPING = {
    { FilterAlgo::AVERAGE, std::make_shared<FilterParameter>("Radius", 0, 10, 1) },
    { FilterAlgo::CONTRAST_ADAPTIVE_SHARPENING, std::make_shared<FilterParameter>("Sharpness", 0, 1, 0.5) },
    { FilterAlgo::GAUSSIAN, std::make_shared<FilterParameter>("Radius", 0, 10, 1) },
    { FilterAlgo::MEDIAN, std::make_shared<FilterParameter>("Radius", 0, 10, 1) },
    { FilterAlgo::SIMPLETRESHOLD, std::make_shared<FilterParameter>("Treshold", 0, 1, 0.1) },
    { FilterAlgo::SOBEL, nullptr },
    { FilterAlgo::OTSU, nullptr }
};

class Filter
{
public:
    virtual std::unique_ptr<Image> Apply(const Image& image) = 0;
    virtual ~Filter() = default;

    static std::unique_ptr<Filter> GetFilter(FilterAlgo filterAlgo, float parameter = 0);
};