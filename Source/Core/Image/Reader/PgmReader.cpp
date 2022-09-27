#include "PgmReader.hpp"
#include "Core/Utils/Utils.hpp"
#include "Core/Log.hpp"

#include <memory>

bool IsDigitChar(char c)
{
    return '0' <= c && c <= '9';
}

void PgmReader::ThrowInvalidHeaderError(size_t pos)
{
    std::string message = std::string("Invalid header of PGM image data. Position: ") + std::to_string(pos) + '.';
    throw std::runtime_error(message);
}

void PgmReader::ThrowInvalidPgmFormatDataError()
{
    throw std::runtime_error("Invalid PGM format data.");
}

PgmHeader PgmReader::ReadHeader(const std::vector<uint8_t>& data)
{
    PgmHeader header;
    bool readingMagic = true;
    bool readingWidth = false;
    bool readingHeight = false;
    bool readingMaxValue = false;

    size_t pos = 0;
    while (pos < data.size()) {
        char curr = (char) data[pos];
        if (readingMagic && curr == '\n') {
            if (pos != 2 || data[0] != 'P' || data[1] != '5') {
                Log::Error("Invalid PGM header at pos {}: header data must start with 'P6' followed by '\\n'.", pos);
                ThrowInvalidHeaderError(pos);
            }
            readingMagic = false;
            readingWidth = true;
        } else if (readingWidth) {
            if (IsDigitChar(curr)) {
                header.width = header.width * 10 + (curr - '0');
            } else if (curr == ' ') {
                readingWidth = false;
                readingHeight = true;
            } else {
                Log::Error("Invalid PPM header at pos {}: reading width, expected: '0'-'9' or ' ', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingHeight) {
            if (IsDigitChar(curr)) {
                header.height = header.height * 10 + (curr - '0');
            } else if (curr == '\n') {
                readingHeight = false;
                readingMaxValue = true;
            } else {
                Log::Error("Invalid PPM header at pos {}: reading height, expected: '0'-'9' or '\\n', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        } else if (readingMaxValue) {
            if (IsDigitChar(curr)) {
                header.maxGreyValue = header.maxGreyValue * 10 + (curr - '0');
            } else if (curr == '\n') {
                readingMaxValue = false;
                break;
            } else {
                Log::Error("Invalid PPM header at pos {}: reading max grey value, expected: '0'-'9' or '\\n', actual: {}", pos, curr);
                ThrowInvalidHeaderError(pos);
            }
        }
        pos++;
    }

    if (readingMagic || readingWidth || readingHeight || readingMaxValue) {
        Log::Error("Invalid PPM header at pos {}. End of header not reached.", pos);
        ThrowInvalidHeaderError(pos);
    }

    header.dataOffset = pos;

    return header;
}

std::shared_ptr<Image> PgmReader::ReadImage(const std::vector<uint8_t>& data)
{
    PgmHeader header = ReadHeader(data);
    Log::Info(
            "Read PGM header: width = {}, height = {}, maxGreyValue = {}, dataOffset = {}.",
            header.width,header.height, header.maxGreyValue, header.dataOffset
    );
    if (data.size() - header.dataOffset - 1 != header.height * header.width) {
        Log::Error(
                "Invalid PGM format data: expected size: {}, actual size: {}.",
                header.height * header.width, data.size() - header.dataOffset - 1
        );
        ThrowInvalidPgmFormatDataError();
    }

    std::vector<Pixel> pixels;
    for (size_t i = header.dataOffset + 1; i < data.size(); i++) {
        if (data[i] > header.maxGreyValue) {
            Log::Error(
                    "Invalid PGM format data: byte value {} is more than header max grey value {}.",
                    (int) data[i],
                    header.maxGreyValue
            );
            ThrowInvalidPgmFormatDataError();
        }
        uint8_t greyByte = Utils::ScaleByte((uint8_t) data[i], header.maxGreyValue);
        Pixel pixel {
            .blue = greyByte,
            .green = greyByte,
            .red = greyByte,
            .alpha = 255u
        };
        pixels.push_back(pixel);
    }

    Log::Info("Successfully read PGM image.");
    return std::make_shared<Image>(header.width, header.height, pixels);
}