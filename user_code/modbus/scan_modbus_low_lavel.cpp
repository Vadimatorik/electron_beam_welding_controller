#include <stdint.h>
#include "uart.h"
#include "timer.h"
#include "pin.h"

#include "stm32f205xx.h"

extern	Uart				scanUartObj;
extern	TimInterrupt		modbusTimInterruptObj;
extern	Pin					scanUartDe;

/// Инициализация UART.
void modBusRtuSlaveUartInit ( uint8_t speed ) {
	( void )speed;
}

/// Разрешить прерывание по окончанию передачи
void modBusRtuSlaveEnableInterTransPhisic ( void ) {
	USART1->CR1		|=	USART_CR1_TCIE;
}

/// Запретить прерывание по окончанию передачи
void modBusRtuSlaveDisableInterTransPhisic ( void ) {
	USART1->CR1		&=	~USART_CR1_TCIE;
}

/// Разрешить прерывание по приему байта.
void modBusRtuSlaveEnableInterReceivPhisic ( void ) {
	USART1->CR1		|=	USART_CR1_RXNEIE;
}

/// Запретить прерывание по приему байта.
void modBusRtuSlaveDisableInterReceivPhisic ( void ) {
	USART1->CR1		&=	~USART_CR1_RXNEIE;
}

/// Передача байта.
void modBusRtuSlaveUartWritePhisic ( uint8_t data ) {
	USART1->DR		=	data;
}

/// Инициализация Таймера.
void modBusRtuSlaveTimerInit ( uint8_t speed ) {
	( void )speed;
}

/// Запустить таймер.
void modBusRtuSlaveTimerStart ( void ) {
	modbusTimInterruptObj.on();
}

/// Стоп таймер.
void modBusRtuSlaveTimerStop ( void ) {
	modbusTimInterruptObj.off();
}

/// Направление линии на прием.
void modBusRtuSlaveRtsRx ( void ) {
	scanUartDe.reset();
}

/// Направление линии на передачу.
void modBusRtuSlaveRtsTx ( void ) {
	scanUartDe.set();
}
