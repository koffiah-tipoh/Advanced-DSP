#include "uart.h"

#define GPIODEN 	(1U<<3)
#define USART3EN 	(1U<<18)
#define CR1_TE 		(1U<<3)
#define CR1_UE		(1U<<13)
#define SR_TXE		(1U<<7)

#define SYS_FREQ 	16000000
#define APB1_CLK 	SYS_FREQ

#define UART_BAUDRATE	115200

static void uart3_set_baudrate(uint32_t periph_clk, uint32_t baudrate);

void uart3_write(int ch);

int __io_putchar(int ch) {/* Re-target printf to use uart3_write() function*/
	uart3_write(ch);
	return ch;
}

void uart3_tx_init(void) {

	/*** Configure UART GPIO Pin***/
	/* Enable clock access to GPIOD */
	RCC->AHB1ENR |= GPIODEN;

	/* Set PD8 mode to Alternate Function mode*/
	GPIOD->MODER &=~ (1U<<16);
	GPIOD->MODER |= (1U<<17);

	/* Set PD8 alternate function type to UART_TX(AF7)*/
	GPIOD->AFR[1] |= (1U<<0);
	GPIOD->AFR[1] |= (1U<<1);
	GPIOD->AFR[1] |= (1U<<2);
	GPIOD->AFR[1] &=~ (1U<<3);

	/*** Configure UART ***/
	/* Enable clock access to UART3 */
	RCC->APB1ENR |= USART3EN;

	/* Configure baud rate */
	uart3_set_baudrate(SYS_FREQ, UART_BAUDRATE);

	/* Configure the transfer direction */
	USART3->CR1 = CR1_TE;

	/* Enable UART module*/
	USART3->CR1 |= CR1_UE;
}

void uart3_write(int ch) {
	/* Make sure transmit data register is empty*/
	while(!(USART3->SR & SR_TXE)) {}

	/* Write to the transmit data register*/
	USART3->DR = (ch & 0xFF);
}


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate) {
	return ((periph_clk + (baudrate/2U))/baudrate);
}

static void uart3_set_baudrate(uint32_t periph_clk, uint32_t baudrate) {
	USART3->BRR = compute_uart_bd(periph_clk, baudrate);
}
