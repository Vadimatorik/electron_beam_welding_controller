#include "uart.h"
#include "pin.h"

extern Pin				scanUartDe;

uartCfg scanUartCfg = {
		.uart				=	USART1,
		.de					=	&scanUartDe,
		.baudrate			=	38400,
		.mode				=	UART_MODE_TX_RX,
		.dmaTx				=	0,
		.dmaTxCh			=	0,
};

Uart	scanUartObj( &scanUartCfg, 1 );
