#include "Resizer.hpp"
#include "BcSpline.hpp"
#include "BilinearResizer.hpp"
#include "Lanczos3Resizer.hpp"
#include "NearestPointResizer.hpp"
#include <stdexcept>

std::unique_ptr<Resizer> Resizer::GetResizer(ResizeAlgo resizer, float* params, uint8_t paramsCount)
{
    switch (resizer) {
        case ResizeAlgo::NEAREST_POINT:
            return std::make_unique<NearestPointResizer>();
        case ResizeAlgo::BILINEAR:
            return std::make_unique<BilinearResizer>();
        case ResizeAlgo::LANCZOS3:
            return std::make_unique<Lanczos3Resizer>();
        case ResizeAlgo::BC:
            if (paramsCount != 2) {
                return std::make_unique<BcSpline>();
            }

            return std::make_unique<BcSpline>(params[0], params[1]);
    }
}
