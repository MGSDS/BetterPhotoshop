#include "Resizer.hpp"
#include <stdexcept>
#include "NearestPointResizer.hpp"

std::unique_ptr<Resizer> Resizer::GetResizer(ResizeAlgo resizer)
{
    switch (resizer) {
        case ResizeAlgo::NEAREST_POINT:
            return std::make_unique<NearestPointResizer>();
        case ResizeAlgo::BILINEAR:
            throw std::logic_error("Not implemented");
        case ResizeAlgo::LANCZOS3:
            throw std::logic_error("Not implemented");
        case ResizeAlgo::BC:
            throw std::logic_error("Not implemented");
    }
}
