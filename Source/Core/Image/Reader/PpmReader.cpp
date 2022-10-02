#include "PpmReader.hpp"
#include "Core/Utils/Utils.hpp"
#include "Core/Log.hpp"

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
    bool readingMaxRedValue = false;
    bool readingMaxGreenValue = false;
    bool readingMaxBlueValue = false;

    size_t pos = 0;
    while (pos < data.size()) {
        char curr = (char) data[pos];
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
                readingMaxRedValue = true;
            } else {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. While reading height, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingMaxRedValue) {
            if (Utils::IsDigit(curr)) {
                header.maxRedValue = header.maxRedValue * 10 + (curr - '0');
            } else if (curr == ' ') {
                readingMaxRedValue = false;
                readingMaxGreenValue = true;
            } else if (curr == '\n') {
                // apply max red value for each color
                header.maxGreenValue = header.maxRedValue;
                header.maxBlueValue = header.maxRedValue;
                readingMaxRedValue = false;
                readingMaxGreenValue = false;
                readingMaxBlueValue = false;
                break;
            } else {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. While reading max red value, expected: '0'-'9' or ' ' or '\\n', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingMaxGreenValue) {
            if (Utils::IsDigit(curr)) {
                header.maxGreenValue = header.maxGreenValue * 10 + (curr - '0');
            } else if (curr == ' ') {
                readingMaxGreenValue = false;
                readingMaxBlueValue = true;
            } else {
                Log::Error("Reading PPM image: invalid PPM header at pos {}. While reading max green value, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingMaxBlueValue) {
            if (Utils::IsDigit(curr)) {
                header.maxBlueValue = header.maxBlueValue * 10 + (curr - '0');
            } else if (curr == '\n') {
                readingMaxBlueValue = false;
                break;
            } else {
                Log::Error("reading PPM image: invalid PPM header at pos {}. While reading max blue value, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        }
        pos++;
    }

    if (
        readingMagic ||
        readingWidth ||
        readingHeight ||
        readingMaxRedValue ||
        readingMaxGreenValue ||
        readingMaxBlueValue
    ) {
        Log::Error("Invalid PPM header at pos {}. End of header not reached.", pos);
        ThrowInvalidHeaderError(pos);
    }

    header.dataOffset = pos;
    return header;
}

std::shared_ptr<Image> PpmReader::ReadImage(const std::vector<uint8_t>& data)
{
    PpmHeader header = PpmReader::ReadHeader(data);
    Log::Info(
            "Reading PPM image: read PPM header. Width = {}, height = {}, maxRedValue = {}, maxGreenValue = {}, maxBlueValue = {}, dataOffset = {}",
            header.width,header.height, header.maxRedValue, header.maxGreenValue, header.maxBlueValue, header.dataOffset
    );

    size_t expectedSize = header.height * header.width * 3;

    if (data.size() - header.dataOffset - 1 != expectedSize) {
        Log::Error(
                "Reading PPM image: invalid PPM format data. Expected size: {}, actual size: {}.",
                expectedSize, data.size() - header.dataOffset
        );
        ThrowInvalidPpmFormatDataError();
    }

    std::vector<Pixel> pixels;
    for (size_t i = header.dataOffset + 1; i < data.size() - 2; i += 3) {
        Pixel pixel {
            .blue = Utils::ScaleByte((uint8_t) data[i + 2], header.maxBlueValue),
            .green = Utils::ScaleByte((uint8_t) data[i + 1], header.maxGreenValue),
            .red = Utils::ScaleByte((uint8_t) data[i], header.maxRedValue),
            .alpha = 255u
        };
        pixels.push_back(pixel);
    }

    Log::Info("Reading PPM image: image successfully read.");
    return std::make_shared<Image>(header.width, header.height, pixels);
}