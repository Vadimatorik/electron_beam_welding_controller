#include <stdint.h>

/// Инициализация UART.
void modBusRtuSlaveUartInit ( uint8_t speed ) {
	( void )speed;
}

/// Разрешить прерывание по окончанию передачи
void modBusRtuSlaveEnableInterTransPhisic ( void ) {

}

/// Запретить прерывание по окончанию передачи
void modBusRtuSlaveDisableInterTransPhisic ( void ) {

}

/// Разрешить прерывание по приему байта.
void modBusRtuSlaveEnableInterReceivPhisic ( void ) {

}

/// Запретить прерывание по приему байта.
void modBusRtuSlaveDisableInterReceivPhisic ( void ) {

}

/// Передача байта.
void modBusRtuSlaveUartWritePhisic ( uint8_t data ) {
	(void)data;
}

/// Инициализация Таймера.
void modBusRtuSlaveTimerInit ( uint8_t speed ) {
	( void )speed;
}

/// Запустить таймер.
void modBusRtuSlaveTimerStart ( void ) {

}

/// Стоп таймер.
void modBusRtuSlaveTimerStop ( void ) {

}

/// Направление линии на прием.
void modBusRtuSlaveRtsRx ( void ) {

}

/// Направление линии на передачу.
void modBusRtuSlaveRtsTx ( void ) {

}
