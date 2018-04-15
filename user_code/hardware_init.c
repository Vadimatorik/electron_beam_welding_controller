#include "stm32f2xx_hal.h"
#include "hardware_init.h"
#include "global_struct.h"

extern globalApplicationStruct		g;

void adcInit( void ) {
	__HAL_RCC_ADC1_CLK_ENABLE();

	ADC_ChannelConfTypeDef sConfig;

	g.adc.Instance							=	ADC1;
	g.adc.Init.ClockPrescaler				=	ADC_CLOCK_SYNC_PCLK_DIV4;
	g.adc.Init.Resolution					=	ADC_RESOLUTION_12B;
	g.adc.Init.ScanConvMode					=	DISABLE;
	g.adc.Init.ContinuousConvMode			=	ENABLE;
	g.adc.Init.DiscontinuousConvMode		=	DISABLE;
	g.adc.Init.ExternalTrigConvEdge			=	ADC_EXTERNALTRIGCONVEDGE_NONE;
	g.adc.Init.ExternalTrigConv				=	ADC_SOFTWARE_START;
	g.adc.Init.DataAlign					=	ADC_DATAALIGN_RIGHT;
	g.adc.Init.NbrOfConversion				=	1;
	g.adc.Init.DMAContinuousRequests		=	DISABLE;
	g.adc.Init.EOCSelection					=	ADC_EOC_SEQ_CONV;

	if ( HAL_ADC_Init(&g.adc) != HAL_OK )
		errorHandler();

	sConfig.Channel							=	ADC_CHANNEL_0;
	sConfig.Rank							=	1;
	sConfig.SamplingTime					=	ADC_SAMPLETIME_28CYCLES;

	if ( HAL_ADC_ConfigChannel(&g.adc, &sConfig) != HAL_OK )
		errorHandler();
}

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

void timInit ( void ) {
	__HAL_RCC_TIM2_CLK_ENABLE();

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	g.tim.Instance							=	TIM2;
	g.tim.Init.Prescaler					=	0;
	g.tim.Init.CounterMode					=	TIM_COUNTERMODE_UP;
	g.tim.Init.Period						=	10000;
	g.tim.Init.ClockDivision				=	TIM_CLOCKDIVISION_DIV1;
	g.tim.Init.AutoReloadPreload			=	TIM_AUTORELOAD_PRELOAD_ENABLE;

	if ( HAL_TIM_Base_Init( &g.tim ) != HAL_OK)
		errorHandler();

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if ( HAL_TIM_ConfigClockSource( &g.tim, &sClockSourceConfig) != HAL_OK)
		errorHandler();

	if ( HAL_TIM_OC_Init( &g.tim ) != HAL_OK )
		errorHandler();

	sMasterConfig.MasterOutputTrigger		=	TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode			=	TIM_MASTERSLAVEMODE_ENABLE;

	if ( HAL_TIMEx_MasterConfigSynchronization( &g.tim, &sMasterConfig ) != HAL_OK )
		errorHandler();

	sConfigOC.OCMode						=	TIM_OCMODE_TIMING;
	sConfigOC.Pulse							=	0;
	sConfigOC.OCPolarity					=	TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode					=	TIM_OCFAST_DISABLE;

	if ( HAL_TIM_OC_ConfigChannel( &g.tim, &sConfigOC, TIM_CHANNEL_1 ) != HAL_OK )
		errorHandler();

	if ( HAL_TIM_OC_ConfigChannel( &g.tim, &sConfigOC, TIM_CHANNEL_2 ) != HAL_OK )
		errorHandler();

	if ( HAL_TIM_OC_ConfigChannel( &g.tim, &sConfigOC, TIM_CHANNEL_3 ) != HAL_OK )
		errorHandler();

	if (HAL_TIM_OC_ConfigChannel( &g.tim, &sConfigOC, TIM_CHANNEL_4 ) != HAL_OK )
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

void gpioForAdcInit ( void ) {
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin						=	GPIO_PIN_0;
	GPIO_InitStruct.Mode					=	GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull					=	GPIO_NOPULL;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
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

