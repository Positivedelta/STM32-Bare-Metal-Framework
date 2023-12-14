//
// (c) Bit Parallel Ltd, September 2023
//

#ifndef BPL_IRQ_LISTENERS_H
#define BPL_IRQ_LISTENERS_H

#include <functional>
#include <memory_resource>
#include <utility>
#include <vector>

namespace bpl
{
    template<class ConsumerType>
    class IrqListeners : public std::pmr::vector<std::reference_wrapper<ConsumerType>>
    {
        public:
            template <typename ...T>
            IrqListeners(T&& ...t) : std::pmr::vector<std::reference_wrapper<ConsumerType>>{{std::forward<T>(t)...}}
            {
            }
    };
}

#endif
