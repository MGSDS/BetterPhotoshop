#include "Utils.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <filesystem>

bool Utils::IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

float Utils::NormByte(uint8_t value, uint8_t maxValue)
{
    return 1.0f * value / maxValue;
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

bool Utils::OutOfBounds(int i, int j, size_t h, size_t w)
{
    return i < 0 || j < 0 || i >= h || j >= w;
}

uint32_t Utils::Crc32(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end)
{
    uint32_t Crc32Table[256];
    for (int i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int j = 0; j < 8; j++) {
            if (c & 1) {
                c = 0xedb88320 ^ (c >> 1);
            }
            else {
                c = c >> 1;
            }
        }
        Crc32Table[i] = c;
    }

    uint32_t crc = 0xFFFFFFFF;
    for (auto b = begin; b != end; b++) {
        crc = (crc >> 8) ^ Crc32Table[(crc ^ *b) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;
}