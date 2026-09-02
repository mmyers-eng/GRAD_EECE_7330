#include <cstdint>

// STM32F4 USART1 Register Addresses
constexpr uint32_t RCC_AHB1ENR = 0x40023830;
constexpr uint32_t RCC_APB2ENR = 0x40023844;
constexpr uint32_t GPIOB_MODER = 0x40020400;
constexpr uint32_t GPIOB_AFRH  = 0x40020424;

constexpr uint32_t USART1_BASE = 0x40011000;
constexpr uint32_t USART1_SR   = USART1_BASE + 0x00;
constexpr uint32_t USART1_DR   = USART1_BASE + 0x04;
constexpr uint32_t USART1_BRR  = USART1_BASE + 0x08;
constexpr uint32_t USART1_CR1  = USART1_BASE + 0x0C;

class UART {
public:
    static void init() {
        // Enable GPIOB and USART1 clocks
        *reinterpret_cast<volatile uint32_t*>(RCC_AHB1ENR) |= (1 << 1); // GPIOBEN
        *reinterpret_cast<volatile uint32_t*>(RCC_APB2ENR) |= (1 << 4); // USART1EN

        // Configure PB6 as Alternate Function (AF7 = USART1_TX)
        volatile uint32_t* gpiob_moder = reinterpret_cast<volatile uint32_t*>(GPIOB_MODER);
        *gpiob_moder &= ~(3 << (6 * 2));
        *gpiob_moder |= (2 << (6 * 2)); // Alt function mode

        volatile uint32_t* gpiob_afrh = reinterpret_cast<volatile uint32_t*>(GPIOB_AFRH);
        *gpiob_afrh &= ~(0xF << 24);
        *gpiob_afrh |= (7 << 24); // AF7 for PB6

        // Set Baud Rate (16MHz clock, 115200 baud -> BRR = 0x8A)
        *reinterpret_cast<volatile uint32_t*>(USART1_BRR) = 0x8A;

        // Enable USART1 & Transmitter (UE | TE)
        *reinterpret_cast<volatile uint32_t*>(USART1_CR1) |= (1 << 13) | (1 << 3);
    }

    static void print_char(char c) {
        // Wait for TXE (Transmit data register empty)
        while (!(*reinterpret_cast<volatile uint32_t*>(USART1_SR) & (1 << 7)));
        *reinterpret_cast<volatile uint32_t*>(USART1_DR) = c;
    }

    static void print_string(const char* str) {
        while (*str) {
            print_char(*str++);
        }
    }
};

extern "C" int main() {
    UART::init();
    UART::print_string("\r\n--- System Booted ---\r\n");

    while (true) {
        // Main loop
    }
    return 0;
}
