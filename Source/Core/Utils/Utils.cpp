#include "Utils.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <cmath>
#include <filesystem>

bool Utils::IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

float Utils::NormByte(uint8_t value, uint8_t maxValue)
{
    return  1.0f * value / maxValue;
}

uint8_t Utils::ByteFromNorm(float norm)
{
    return static_cast<uint8_t>(round(255 * std::clamp(norm, 0.0f, 1.0f)));
}

std::vector<uint8_t> Utils::ReadAllBytes(std::ifstream& ifs) 
{
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<uint8_t> fileData(size);
    if (!ifs.read(reinterpret_cast<char*>(&fileData[0]), size)) {
        throw std::runtime_error("Error while reading file.");
    }

    return fileData;
}

std::string Utils::ToLower(const std::string& str)
{
    std::string result;
    result.reserve(str.size());

    for (char c : str) {
        result.push_back(std::tolower(c));
    }

    return result;
}

std::string Utils::GetFileExtension(const std::string& path)
{
    std::filesystem::path p(path);
    if (std::filesystem::is_directory(p)) {
        return std::string();
    }

    return p.extension().string();
}

bool Utils::DataStartsWith(const std::vector<uint8_t>& data, const std::vector<uint8_t>& prefix)
{
    return data.size() >= prefix.size() && memcmp(&data[0], &prefix[0], prefix.size()) == 0;
}