//
// (c) Bit Parallel Ltd, 2022
//

#ifndef BPL_BYTE_LISTENER_H
#define BPL_BYTE_LISTENER_H

#include <cstdint>
#include <functional>

namespace bpl
{
    // a return value of true is intended to indicate that the byte has been fully handled and no futher action is required
    //
    using ByteListener = std::function<bool (const uint8_t byte)>;
}

#endif
