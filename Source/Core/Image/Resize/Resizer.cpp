#include "Resizer.hpp"
#include <stdexcept>
#include "NearestPointResizer.hpp"
#include "BilinearResizer.hpp"
#include "Lanczos3Resizer.hpp"

std::unique_ptr<Resizer> Resizer::GetResizer(ResizeAlgo resizer)
{
    switch (resizer) {
        case ResizeAlgo::NEAREST_POINT:
            return std::make_unique<NearestPointResizer>();
        case ResizeAlgo::BILINEAR:
            return std::make_unique<BilinearResizer>();
        case ResizeAlgo::LANCZOS3:
            return std::make_unique<Lanczos3Resizer>();
        case ResizeAlgo::BC:
            throw std::logic_error("Not implemented");
    }
}
