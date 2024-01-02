//
// (c) Bit Parallel Ltd, January 2024
//

#ifndef BPL_STM32F4_EXTI_H
#define BPL_STM32F4_EXTI_H

class Exti
{
    public:
        inline static constexpr uint32_t IMR = 0x00;
        inline static constexpr uint32_t IMR_MR0 = 1;
        inline static constexpr uint32_t IMR_MR1 = 1 << 1;
        inline static constexpr uint32_t IMR_MR2 = 1 << 2;
        inline static constexpr uint32_t IMR_MR3 = 1 << 3;
        inline static constexpr uint32_t IMR_MR4 = 1 << 4;
        inline static constexpr uint32_t IMR_MR5 = 1 << 5;
        inline static constexpr uint32_t IMR_MR6 = 1 << 6;
        inline static constexpr uint32_t IMR_MR7 = 1 << 7;
        inline static constexpr uint32_t IMR_MR8 = 1 << 8;
        inline static constexpr uint32_t IMR_MR9 = 1 << 9;
        inline static constexpr uint32_t IMR_MR10 = 1 << 10;
        inline static constexpr uint32_t IMR_MR11 = 1 << 11;
        inline static constexpr uint32_t IMR_MR12 = 1 << 12;
        inline static constexpr uint32_t IMR_MR13 = 1 << 13;
        inline static constexpr uint32_t IMR_MR14 = 1 << 14;
        inline static constexpr uint32_t IMR_MR15 = 1 << 15;
        inline static constexpr uint32_t IMR_MR16 = 1 << 16;
        inline static constexpr uint32_t IMR_MR17 = 1 << 17;
        inline static constexpr uint32_t IMR_MR18 = 1 << 18;
        inline static constexpr uint32_t IMR_MR19 = 1 << 19;
        inline static constexpr uint32_t IMR_MR20 = 1 << 20;
        inline static constexpr uint32_t IMR_MR21 = 1 << 21;
        inline static constexpr uint32_t IMR_MR22 = 1 << 22;

        // note, RTSR_TR19 is reserved, i.e. missing
        //
        inline static constexpr uint32_t RTSR = 0x08;
        inline static constexpr uint32_t RTSR_TR0 = 1;
        inline static constexpr uint32_t RTSR_TR1 = 1 << 1;
        inline static constexpr uint32_t RTSR_TR2 = 1 << 2;
        inline static constexpr uint32_t RTSR_TR3 = 1 << 3;
        inline static constexpr uint32_t RTSR_TR4 = 1 << 4;
        inline static constexpr uint32_t RTSR_TR5 = 1 << 5;
        inline static constexpr uint32_t RTSR_TR6 = 1 << 6;
        inline static constexpr uint32_t RTSR_TR7 = 1 << 7;
        inline static constexpr uint32_t RTSR_TR8 = 1 << 8;
        inline static constexpr uint32_t RTSR_TR9 = 1 << 9;
        inline static constexpr uint32_t RTSR_TR10 = 1 << 10;
        inline static constexpr uint32_t RTSR_TR11 = 1 << 11;
        inline static constexpr uint32_t RTSR_TR12 = 1 << 12;
        inline static constexpr uint32_t RTSR_TR13 = 1 << 13;
        inline static constexpr uint32_t RTSR_TR14 = 1 << 14;
        inline static constexpr uint32_t RTSR_TR15 = 1 << 15;
        inline static constexpr uint32_t RTSR_TR16 = 1 << 16;
        inline static constexpr uint32_t RTSR_TR17 = 1 << 17;
        inline static constexpr uint32_t RTSR_TR18 = 1 << 18;
        inline static constexpr uint32_t RTSR_TR20 = 1 << 20;
        inline static constexpr uint32_t RTSR_TR21 = 1 << 21;
        inline static constexpr uint32_t RTSR_TR22 = 1 << 22;

        inline static constexpr uint32_t PR = 0x14;
        inline static constexpr uint32_t PR_PR0 = 1;
        inline static constexpr uint32_t PR_PR1 = 1 << 1;
        inline static constexpr uint32_t PR_PR2 = 1 << 2;
        inline static constexpr uint32_t PR_PR3 = 1 << 3;
        inline static constexpr uint32_t PR_PR4 = 1 << 4;
        inline static constexpr uint32_t PR_PR5 = 1 << 5;
        inline static constexpr uint32_t PR_PR6 = 1 << 6;
        inline static constexpr uint32_t PR_PR7 = 1 << 7;
        inline static constexpr uint32_t PR_PR8 = 1 << 8;
        inline static constexpr uint32_t PR_PR9 = 1 << 9;
        inline static constexpr uint32_t PR_PR10 = 1 << 10;
        inline static constexpr uint32_t PR_PR11 = 1 << 11;
        inline static constexpr uint32_t PR_PR12 = 1 << 12;
        inline static constexpr uint32_t PR_PR13 = 1 << 13;
        inline static constexpr uint32_t PR_PR14 = 1 << 14;
        inline static constexpr uint32_t PR_PR15 = 1 << 15;
        inline static constexpr uint32_t PR_PR16 = 1 << 16;
        inline static constexpr uint32_t PR_PR17 = 1 << 17;
        inline static constexpr uint32_t PR_PR18 = 1 << 18;
        inline static constexpr uint32_t PR_PR19 = 1 << 19;
        inline static constexpr uint32_t PR_PR20 = 1 << 20;
        inline static constexpr uint32_t PR_PR21 = 1 << 21;
        inline static constexpr uint32_t PR_PR22 = 1 << 22;
};

#endif
