#pragma once

#include <cstdint>
#include <vector>
#include <fstream>
#include <string>


class Utils {
public:
    static bool IsDigit(char c);
    static float NormByte(uint8_t value, uint8_t maxValue);
    static std::vector<uint8_t> ReadAllBytes(std::ifstream& ifs);
    static std::string ToLower(const std::string& str);
    static std::string GetFileExtension(const std::string& path);
    static bool DataStartsWith(const std::vector<uint8_t>& data, const std::vector<uint8_t>& prefix);
};
