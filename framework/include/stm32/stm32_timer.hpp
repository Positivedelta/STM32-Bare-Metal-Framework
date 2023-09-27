//
// (c) Bit Parallel Ltd, 2022
//

//
// FIXME! only tested on timer4, will hopefully this will be generic...
//

#ifndef BPL_STM32TIMER_H
#define BPL_STM32TIMER_H

class Timer
{
    public:
        inline static constexpr uint32_t CR1 = 0x00;
        inline static constexpr uint32_t CR1_CEN = 1;
        inline static constexpr uint32_t CR1_UDIS = 1 << 1;
        inline static constexpr uint32_t CR1_URS = 1 << 2;
        inline static constexpr uint32_t CR1_OPM = 1 << 3;
        inline static constexpr uint32_t CR1_DIR = 1 << 4;
        inline static constexpr uint32_t CR1_ARPE = 1 << 7;

        inline static constexpr uint32_t CR1_CMS_EDGE_ALIGNED = 0x00;
        inline static constexpr uint32_t CR1_CMS_CENTER_ALIGNED_1 = 0x01 << 5;
        inline static constexpr uint32_t CR1_CMS_CENTER_ALIGNED_2 = 0x02 << 5;
        inline static constexpr uint32_t CR1_CMS_CENTER_ALIGNED_3 = 0x03 << 5;

        inline static constexpr uint32_t CR1_CKD_1 = 0x00;
        inline static constexpr uint32_t CR1_CKD_2 = 0x01 << 8;
        inline static constexpr uint32_t CR1_CKD_4 = 0x02 << 8;

        inline static constexpr uint32_t EGR = 0x14;
        inline static constexpr uint32_t EGR_UG = 1;

        inline static constexpr uint32_t CCMR1 = 0x18;
        inline static constexpr uint32_t CCMR1_OC1PE = 1 << 3;
        inline static constexpr uint32_t CCMR1_OC1M_PWM_MODE_1 = 0x06 << 4;
        inline static constexpr uint32_t CCMR1_OC2PE = 1 << 11;
        inline static constexpr uint32_t CCMR1_OC2M_PWM_MODE_1 = 0x06 << 12;

        inline static constexpr uint32_t CCMR2 = 0x1c;
        inline static constexpr uint32_t CCMR2_OC3PE = 1 << 3;
        inline static constexpr uint32_t CCMR2_OC3M_PWM_MODE_1 = 0x06 << 4;
        inline static constexpr uint32_t CCMR2_OC4PE = 1 << 11;
        inline static constexpr uint32_t CCMR2_OC4M_PWM_MODE_1 = 0x06 << 12;

        inline static constexpr uint32_t CCER = 0x20;
        inline static constexpr uint32_t CCER_CC1E = 1;
        inline static constexpr uint32_t CCER_CC2E = 1 << 4;
        inline static constexpr uint32_t CCER_CC3E = 1 << 8;
        inline static constexpr uint32_t CCER_CC4E = 1 << 12;

        inline static constexpr uint32_t CNT = 0x24;
        inline static constexpr uint32_t PSC = 0x28;
        inline static constexpr uint32_t ARR = 0x2c;

        inline static constexpr uint32_t CCR1 = 0x34;
        inline static constexpr uint32_t CCR2 = 0x38;
        inline static constexpr uint32_t CCR3 = 0x3c;
        inline static constexpr uint32_t CCR4 = 0x40;
};

#endif
