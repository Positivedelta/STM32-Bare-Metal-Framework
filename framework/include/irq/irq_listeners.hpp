//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_IRQ_LISTENERS_H
#define BPL_IRQ_LISTENERS_H

#include <functional>
#include <memory_resource>
#include <utility>
#include <vector>

#include "irq_consumer.hpp"

namespace bpl
{
    using Consumer = std::reference_wrapper<IrqConsumer>;

    class IrqListeners : public std::pmr::vector<Consumer>
    {
        public:
            template <typename ...T>
            IrqListeners(T&& ...t) : std::pmr::vector<Consumer>{{std::forward<T>(t)...}}
            {
            }
    };
}

#endif
