#include "FormatReaderResolver.hpp"
#include "PgmReader.hpp"
#include "PpmReader.hpp"

#include <stdexcept>

std::shared_ptr<ImageReader> FormatReaderResolver::GetReader(const std::vector<uint8_t>& data)
{
    if (data.size() < 2) {
        throw std::invalid_argument("Unknown image format.");
    }
    if ((char) data[0] != 'P') {
        throw std::invalid_argument("Unknown image format.");
    }
    switch ((char) data[1]) {
        case '5': return std::shared_ptr<ImageReader>(new PgmReader());
        case '6': return std::shared_ptr<ImageReader>(new PpmReader());
        default: throw std::invalid_argument("Unknown image format.");
    }
}
