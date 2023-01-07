#pragma once

#include <Core/Image/Image.hpp>

enum ResizeAlgo : uint8_t
{
    NEAREST_POINT,
    BILINEAR,
    LANCZOS3,
    BC,
};

static const std::unordered_map<ResizeAlgo, std::string> ENUM_TO_STRING_RESIZER_MAPPING = {
    {ResizeAlgo::NEAREST_POINT, "Nearest point"},
    {ResizeAlgo::BILINEAR, "Bilinear"},
    {ResizeAlgo::LANCZOS3, "Lanczos3"},
    {ResizeAlgo::BC, "BC-spline"}
};

class Resizer
{
public:
    virtual ~Resizer() = default;

    virtual std::unique_ptr<Image> Apply(const Image& img, float scaleX, float scaleY) = 0;

    static std::unique_ptr<Resizer> GetResizer(ResizeAlgo resizer, float* params = nullptr, uint8_t paramsCount = 0);
};
