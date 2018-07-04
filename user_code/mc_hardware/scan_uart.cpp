#include "uart.h"

uartCfg scanUartCfg[] = {
	{
		.uart				=	USART1,
		.baudrate			=	115200,
		.mode				=	UART_MODE_TX_RX,
		.dmaTx				=	0,
		.dmaTxCh			=	0
	}
};

Uart	scanUartObj( scanUartCfg, M_SIZE_ARRAY( scanUartCfg ) );
