//
// (c) Bit Parallel Ltd, 2021
//

#ifndef BPL_STM32GPIO_H
#define BPL_STM32GPIO_H

class Gpio
{
    public:
        inline static constexpr uint32_t Pin0 = 0x00;
        inline static constexpr uint32_t Pin1 = 0x01;
        inline static constexpr uint32_t Pin2 = 0x02;
        inline static constexpr uint32_t Pin3 = 0x03;
        inline static constexpr uint32_t Pin4 = 0x04;
        inline static constexpr uint32_t Pin5 = 0x05;
        inline static constexpr uint32_t Pin6 = 0x06;
        inline static constexpr uint32_t Pin7 = 0x07;
        inline static constexpr uint32_t Pin8 = 0x08;
        inline static constexpr uint32_t Pin9 = 0x09;
        inline static constexpr uint32_t Pin10 = 0x0a;
        inline static constexpr uint32_t Pin11 = 0x0b;
        inline static constexpr uint32_t Pin12 = 0x0c;
        inline static constexpr uint32_t Pin13 = 0x0d;
        inline static constexpr uint32_t Pin14 = 0x0e;
        inline static constexpr uint32_t Pin15 = 0x0f;

        inline static constexpr uint32_t MODER = 0x00;
        inline static constexpr uint32_t MODER_MASK = 0b11;
        inline static constexpr uint32_t MODER_SHIFT = 1;
        inline static constexpr uint32_t IN = 0x00;
        inline static constexpr uint32_t OP = 0x01;
        inline static constexpr uint32_t AF = 0x02;
        inline static constexpr uint32_t AN = 0x03;

        inline static constexpr uint32_t OTYPER = 0x04;
        inline static constexpr uint32_t PushPull = 0x00;
        inline static constexpr uint32_t OpenD = 0x01;

        inline static constexpr uint32_t OSPEEDR = 0x08;
        inline static constexpr uint32_t OSPEEDR_MASK = 0b11;
        inline static constexpr uint32_t OSPEEDR_SHIFT = 1;
        inline static constexpr uint32_t Low = 0x00;
        inline static constexpr uint32_t Med = 0x01;
        inline static constexpr uint32_t Fast = 0x02;
        inline static constexpr uint32_t High = 0x03;

        inline static constexpr uint32_t PUPDR = 0x0c;
        inline static constexpr uint32_t PUPDR_MASK = 0b11;
        inline static constexpr uint32_t PUPDR_SHIFT = 1;
        inline static constexpr uint32_t None = 0x00;
        inline static constexpr uint32_t Pup = 0x01;
        inline static constexpr uint32_t Pdwn = 0x02;

        inline static constexpr uint32_t IDR = 0x10;
        inline static constexpr uint32_t ODR = 0x14;
        inline static constexpr uint32_t BSR = 0x18;
        inline static constexpr uint32_t BSR_RESET_OFFSET = 0x10;
        inline static constexpr uint32_t LCKR = 0x1c;

        inline static constexpr uint32_t AFRL = 0x20;
        inline static constexpr uint32_t AFRH = 0x24;
        inline static constexpr uint32_t AFR_MASK = 0b1111;
        inline static constexpr uint32_t AFR_SHIFT = 2;

        inline static constexpr uint32_t AF0 = 0x00;
        inline static constexpr uint32_t AF1 = 0x01;
        inline static constexpr uint32_t AF2 = 0x02;
        inline static constexpr uint32_t AF3 = 0x03;
        inline static constexpr uint32_t AF4 = 0x04;
        inline static constexpr uint32_t AF5 = 0x05;
        inline static constexpr uint32_t AF6 = 0x06;
        inline static constexpr uint32_t AF7 = 0x07;
        inline static constexpr uint32_t AF8 = 0x08;
        inline static constexpr uint32_t AF9 = 0x09;
        inline static constexpr uint32_t AF10 = 0x0a;
        inline static constexpr uint32_t AF11 = 0x0b;
        inline static constexpr uint32_t AF12 = 0x0c;
        inline static constexpr uint32_t AF13 = 0x0d;
        inline static constexpr uint32_t AF14 = 0x0e;
        inline static constexpr uint32_t AF15 = 0x0f;
};

#endif
