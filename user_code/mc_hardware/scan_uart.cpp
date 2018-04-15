#include "uart.h"

uartCfg scanUartCfg = {
	.uart				=	USART1,
	.baudrate			=	115200,
	.mode				=	UART_MODE_TX_RX,
	.dmaTx				=	DMA2_Stream7,
	.dmaTxCh			=	DMA_CHANNEL_4
};

Uart	scanUartObj( &scanUartCfg );
