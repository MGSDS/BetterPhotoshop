#include "PpmReader.hpp"
#include "Core/Log.hpp"
#include "Core/Utils/Utils.hpp"

#include <string>

void PpmReader::ThrowInvalidHeaderError(size_t pos)
{
    std::string message = std::string("Invalid header of PPM image data. Position: ") + std::to_string(pos) + '.';
    throw std::runtime_error(message);
}

void PpmReader::ThrowInvalidPpmFormatDataError()
{
    throw std::runtime_error("Invalid PPM format data.");
}

PpmHeader PpmReader::ReadHeader(const std::vector<uint8_t>& data)
{
    PpmHeader header;
    bool readingMagic = true;
    bool readingWidth = false;
    bool readingHeight = false;
    bool readingMaxCh0Value = false;
    bool readingMaxCh1Value = false;
    bool readingMaxCh2Value = false;

    size_t pos = 0;
    while (pos < data.size()) {
        char curr = (char)data[pos];
        if (readingMagic && curr == '\n') {
            if (pos != 2 || data[0] != 'P' || data[1] != '6') {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. Header data must start with 'P6' followed by '\\n'.", pos);
                ThrowInvalidHeaderError(pos);
            }
            readingMagic = false;
            readingWidth = true;
        } else if (readingWidth) {
            if (Utils::IsDigit(curr)) {
                header.width = header.width * 10 + (curr - '0');
            } else if (curr == ' ') {
                readingWidth = false;
                readingHeight = true;
            } else {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. While reading width, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingHeight) {
            if (Utils::IsDigit(curr)) {
                header.height = header.height * 10 + (curr - '0');
            } else if (curr == '\n') {
                readingHeight = false;
                readingMaxCh0Value = true;
            } else {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. While reading height, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingMaxCh0Value) {
            if (Utils::IsDigit(curr)) {
                header.maxCh0Value = header.maxCh0Value * 10 + (curr - '0');
            } else if (curr == ' ') {
                readingMaxCh0Value = false;
                readingMaxCh1Value = true;
            } else if (curr == '\n') {
                // apply max ch0 value for each color
                header.maxCh1Value = header.maxCh0Value;
                header.maxCh2Value = header.maxCh0Value;
                readingMaxCh0Value = false;
                readingMaxCh1Value = false;
                readingMaxCh2Value = false;
                break;
            } else {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. While reading max ch0 value, expected: '0'-'9' or ' ' or '\\n', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingMaxCh1Value) {
            if (Utils::IsDigit(curr)) {
                header.maxCh1Value = header.maxCh1Value * 10 + (curr - '0');
            } else if (curr == ' ') {
                readingMaxCh1Value = false;
                readingMaxCh2Value = true;
            } else {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. While reading max ch1 value, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingMaxCh2Value) {
            if (Utils::IsDigit(curr)) {
                header.maxCh2Value = header.maxCh2Value * 10 + (curr - '0');
            } else if (curr == '\n') {
                readingMaxCh2Value = false;
                break;
            } else {
                Log::Error("reading PPM image: invalid PPM header at pos {}. While reading max ch2 value, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        }
        pos++;
    }

    if (
        readingMagic ||
        readingWidth ||
        readingHeight ||
        readingMaxCh0Value ||
        readingMaxCh1Value ||
        readingMaxCh2Value) {
        Log::Error("Invalid PPM header at pos {}. End of header not reached.", pos);
        ThrowInvalidHeaderError(pos);
    }

    header.dataOffset = pos;
    return header;
}

std::unique_ptr<Image> PpmReader::ReadImage(const std::vector<uint8_t>& data)
{
    PpmHeader header = PpmReader::ReadHeader(data);
    Log::Info(
        "Reading PPM image: read PPM header. Width = {}, height = {}, maxCh0Value = {}, maxCh1Value = {}, maxCh2Value = {}, dataOffset = {}",
        header.width, header.height, header.maxCh0Value, header.maxCh1Value, header.maxCh2Value, header.dataOffset);

    size_t expectedSize = header.height * header.width * 3;

    if (data.size() - header.dataOffset - 1 != expectedSize) {
        Log::Error(
            "Reading PPM image: invalid PPM format data. Expected size: {}, actual size: {}.",
            expectedSize, data.size() - header.dataOffset);
        ThrowInvalidPpmFormatDataError();
    }

    std::vector<Pixel> pixels;
    for (size_t i = header.dataOffset + 1; i < data.size() - 2; i += 3) {
        float ch0 = Utils::NormByte(data[i], header.maxCh0Value);
        float ch1 = Utils::NormByte(data[i + 1], header.maxCh1Value);
        float ch2 = Utils::NormByte(data[i + 2], header.maxCh2Value);
        pixels.emplace_back(ch0, ch1, ch2, 1.0f);
    }

    Log::Info("Reading PPM image: image successfully read.");
    return std::make_unique<Image>(header.width, header.height, pixels);
}