//
// (c) Bit Parallel Ltd, 2022
//

#ifndef BPL_STM32FPU_H
#define BPL_STM32FPU_H

class Fpu
{
    public:
        inline static constexpr uint32_t CPACR = 0x00;
        inline static constexpr uint32_t CPACR_CP10_ACCESS_DENIED = 0x00;
        inline static constexpr uint32_t CPACR_CP10_PRIVILEGED = 0x01 << 20;
        inline static constexpr uint32_t CPACR_CP10_RESERVED = 0x02 << 20;
        inline static constexpr uint32_t CPACR_CP10_FULL_ACCESS = 0x03 << 20;
        inline static constexpr uint32_t CPACR_CP11_ACCESS_DENIED = 0x00;
        inline static constexpr uint32_t CPACR_CP11_PRIVILEGED = 0x01 << 20;
        inline static constexpr uint32_t CPACR_CP11_RESERVED = 0x02 << 20;
        inline static constexpr uint32_t CPACR_CP11_FULL_ACCESS = 0x03 << 20;

        inline static constexpr uint32_t FPCCR = 0x1ac;
        inline static constexpr uint32_t FPCAR = 0x1b0;
        inline static constexpr uint32_t FPDSCR = 0x1b4;

        //
        // utility functions
        //

        __attribute__((always_inline))
        static inline float sqrt(float value)
        {
            // this instruction takes at most 14 cycles
            //
            float result;
            asm volatile ("vsqrt.f32 %0, %1" : "=&t" (result) : "t" (value));

            return result;
        }
};

#endif
