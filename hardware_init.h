#pragma once

void errorHandler			( void );

void systemClockConfig		( void );
void adcInit				( void );
void dacInit				( void );
void timInit				( void );
void uartInit				( void );

void gpioForAdcInit			( void );
void gpioForDacInit			( void );
void gpioForUartInit		( void );
void gpioForExtiInit		( void );

void nvicInit				( void );

void hardwareInit			( void );
