#include "stm32f2xx_hal.h"
#include "hardware_init.h"
#include "global_struct.h"

extern globalApplicationStruct		g;

void dacInit ( void ) {
	__HAL_RCC_DAC_CLK_ENABLE();

	DAC_ChannelConfTypeDef sConfig;

	g.dac.Instance = DAC;

	if ( HAL_DAC_Init( &g.dac ) != HAL_OK )
		errorHandler();

	sConfig.DAC_Trigger						= DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer				= DAC_OUTPUTBUFFER_ENABLE;

	if ( HAL_DAC_ConfigChannel( &g.dac, &sConfig, DAC_CHANNEL_1 ) != HAL_OK)
		errorHandler();

	if ( HAL_DAC_ConfigChannel( &g.dac, &sConfig, DAC_CHANNEL_2 ) != HAL_OK)
		errorHandler();
}

void uartInit ( void ) {
	__HAL_RCC_USART1_CLK_ENABLE();

	g.uart.Instance							= USART1;
	g.uart.Init.BaudRate					= 9600;
	g.uart.Init.WordLength					= UART_WORDLENGTH_8B;
	g.uart.Init.StopBits					= UART_STOPBITS_1;
	g.uart.Init.Parity						= UART_PARITY_NONE;
	g.uart.Init.Mode						= UART_MODE_TX_RX;
	g.uart.Init.HwFlowCtl					= UART_HWCONTROL_NONE;
	g.uart.Init.OverSampling				= UART_OVERSAMPLING_16;
	if ( HAL_UART_Init( &g.uart ) != HAL_OK )
		errorHandler();
}

void nvicInit ( void ) {
	HAL_NVIC_SetPriority(ADC_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

void hardwareInit ( void ) {
	HAL_Init();

	gpioForAdcInit();
	gpioForDacInit();
	gpioForUartInit();
	gpioForExtiInit();

	systemClockConfig();

	adcInit();
	dacInit();
	timInit();
	uartInit();

	nvicInit();
}

