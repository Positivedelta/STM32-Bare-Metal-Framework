//
// (c) Bit Parallel Ltd, 2022
//

#ifndef BPL_BYTE_LISTENER_H
#define BPL_BYTE_LISTENER_H

#include <cstdint>
#include <functional>

namespace bpl
{
    using ByteListener = const std::function<void (const uint8_t byte)>;
}

#endif
