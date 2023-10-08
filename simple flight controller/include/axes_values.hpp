//
// (c) Bit Parallel Ltd, October 2023
//

#ifndef BPL_AXES_VALUES_H
#define BPL_AXES_VALUES_H

#include <cstdint>

class AxesValues
{
    private:
        int32_t x, y, z;

    public:
        AxesValues(const int32_t x, const int32_t y, const int32_t z);
        const int32_t getX() const;
        const int32_t getY() const;
        const int32_t getZ() const;
};

#endif
