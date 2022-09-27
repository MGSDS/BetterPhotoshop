#include "Utils.hpp"

#include <cctype>
#include <filesystem>

bool Utils::IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

uint8_t Utils::ScaleByte(uint8_t value, uint8_t maxValue) 
{
    return (uint8_t)(((uint32_t) value) * maxValue / 255u);
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

