#include "driver_uart.h"
#include <stdint.h>

#include "stm32f405xx.h"

void uart_init(void)
{
        // Enable GPIOB and USART1 clocks
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

        // Configure PB6 as Alternate Function (AF7 = USART1_TX)
        GPIOB->MODER &= ~(GPIO_MODER_MODER6_Msk);
        GPIOB->MODER |= GPIO_MODER_MODER6_1;

        GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk);
        GPIOB->AFR[0] |= (7UL << GPIO_AFRL_AFSEL6_Pos); // AF7 for PB6

	// Ensure USART is disabled while configuring
	USART1->CR1 &= ~USART_CR1_UE;
	NVIC_EnableIRQ(USART1_IRQn);
        // Set Baud Rate (16MHz clock, 115200 baud -> BRR = 0x8A)
        USART1->BRR = 0x8A;
	USART1->CR1 &= ~(USART_CR1_M | USART_CR1_PCE);
	USART1->CR2 &= ~USART_CR2_STOP;

        // Enable USART1 & Transmitter (UE | TE)
        USART1->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE);
}



static void uart_base_init(const uint32_t uart_base)
{
        (void)uart_base;

        // Configure PB6 as Alternate Function (AF7 = USART1_TX)
        GPIOB->MODER &= ~(GPIO_MODER_MODER6_Msk);
        GPIOB->MODER |= GPIO_MODER_MODER6_1;

        GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk);
        GPIOB->AFR[0] |= (7UL << GPIO_AFRL_AFSEL6_Pos); // AF7 for PB6

        // Set Baud Rate (16MHz clock, 115200 baud -> BRR = 0x8A)
        USART1->BRR = 0x8A;

        // Enable USART1 & Transmitter (UE | TE)
        USART1->CR1 |= (USART_CR1_UE | USART_CR1_TE);
}


void uart_print_char(char c)
{
        // Wait for TXE (Transmit data register empty)
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = c;
}

void USART1_IRQHandler()
{
	if (USART1->SR & USART_SR_RXNE)
	{
                //get the received character from the data register
		char c = (char)(USART1->DR & 0xFF);
                // immediately transmit the received character back (echo)
		uart_print_char(c);
	}	
}

int uart_get_char(char *c)
{
        // Wait for TXE (Transmit data register empty)
        //while (!(USART1->SR & USART_SR_TXE));
        //USART1->DR = c;
}

void uart_loop_back_test(void)
{

	printf("start loopback \n\r");
	while (1) {
		// 1. Wait until the Receive Data Register Not Empty (RXNE) flag is set
		while (!(USART1->SR & USART_SR_RXNE));
		// 2. Read the received byte (reading DR automatically clears RXNE)
		char c = (char)(USART1->DR & 0xFF);

		// 3. Wait until the Transmit Data Register Empty (TXE) flag is set
		while (!(USART1->SR & USART_SR_TXE));

		// 4. Write the character back out to the Data Register
		USART1->DR = c;
	}

}
