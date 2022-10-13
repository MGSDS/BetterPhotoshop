#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class ImageFormat : uint32_t
{
    Pgm = 0,
    Ppm,
};

struct ImageFormatInfo
{
    ImageFormat Format;
    std::string FileExtension;
};

static std::unordered_map<std::string, ImageFormatInfo> FILTER_TO_IMAGE_FORMAT_INFO_MAPPING = {
    { "PGM Image (*.pgm)", { ImageFormat::Pgm, ".pgm" } },
    { "PPM Image (*.ppm)", { ImageFormat::Ppm, ".ppm" } },
};
