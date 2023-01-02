#include "Resizer.hpp"
#include <stdexcept>
#include "NearestPointResizer.hpp"
#include "BilinearResizer.hpp"

std::unique_ptr<Resizer> Resizer::GetResizer(ResizeAlgo resizer)
{
    switch (resizer) {
        case ResizeAlgo::NEAREST_POINT:
            return std::make_unique<NearestPointResizer>();
        case ResizeAlgo::BILINEAR:
            return std::make_unique<BilinearResizer>();
        case ResizeAlgo::LANCZOS3:
            throw std::logic_error("Not implemented");
        case ResizeAlgo::BC:
            throw std::logic_error("Not implemented");
    }
}
