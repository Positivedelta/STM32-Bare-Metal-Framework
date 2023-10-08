//
// (c) Bit Parallel Ltd, October 2023
//

#include "axes_values.hpp"

AxesValues::AxesValues(const int32_t x, const int32_t y, const int32_t z):
    x(x), y(y), z(z) {
}

const int32_t AxesValues::getX() const
{
    return x;
}

const int32_t AxesValues::getY() const
{
    return y;
}

const int32_t AxesValues::getZ() const
{
    return z;
}
