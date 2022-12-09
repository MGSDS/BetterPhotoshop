#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class Utils
{
public:
    static bool IsDigit(char c);
    static float NormByte(uint8_t value, uint8_t maxValue);
    static uint8_t ByteFromNorm(float norm);
    static std::vector<uint8_t> ReadAllBytes(std::ifstream& ifs);
    static std::string ToLower(const std::string& str);
    static std::string GetFileExtension(const std::string& path);
    static bool DataStartsWith(const std::vector<uint8_t>& data, const std::vector<uint8_t>& prefix);
    static bool OutOfBounds(int i, int j, size_t h, size_t w);
    static uint32_t Crc32(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end);
    static std::vector<uint32_t> ToByteArray(uint32_t data);
};