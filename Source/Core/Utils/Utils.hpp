#pragma once

#include <cstdint>
#include <vector>
#include <fstream>


class Utils {
public:
    static bool IsDigit(char c);
    static uint8_t ScaleByte(uint8_t value, uint8_t maxValue);
    static std::vector<uint8_t> ReadAllBytes(std::ifstream& ifs);
};
