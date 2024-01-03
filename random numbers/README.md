## Basic Operation
- The Entropy class configures the RTC wakeup down counter to run from the internal 38KHz RC oscillator and triggers an IRQ each time it cycles
- The RC oscillator is not particularly accurate and drifts, so can be used as a reasonable source of entropy
- Timer 3 is runs from the much more accurate crystal controlled clock and is sampled on each RTC wakeup IRQ, thus generating random values
- For the full details, please refer to the `entropy.cpp` source code

## Frequency Histogram Plot
- 200,250 Random numbers
- Generated in the range 0 to 999
- Perhaps not suitable for cryptographically secure applications, but otherwise quite usable!

<img src='https://github.com/Positivedelta/STM32-Bare-Metal-Framework/blob/6e6cbd584ac04ed63c765fe7f68450ef9d06ce70/random%20numbers/entropy_200250_1000.png'>
