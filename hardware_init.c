#include "hardware_init.h"
#include "stm32f2xx_hal.h"
#include "global_struct.h"

extern globalApplicationStruct		g;

void systemClockConfig ( void ) {
	RCC_OscInitTypeDef	RCC_OscInitStruct;
	RCC_ClkInitTypeDef	RCC_ClkInitStruct;

	RCC_OscInitStruct.OscillatorType		=	RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState				=	RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState			=	RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource			=	RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM				=	20;
	RCC_OscInitStruct.PLL.PLLN				=	192;
	RCC_OscInitStruct.PLL.PLLP				=	RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ				=	4;

	if ( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
		errorHandler();

	RCC_ClkInitStruct.ClockType				=	RCC_CLOCKTYPE_HCLK |	RCC_CLOCKTYPE_SYSCLK |
												RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider			=	RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider		=	RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider		=	RCC_HCLK_DIV2;

	if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_3 ) != HAL_OK )
		errorHandler();
}

void adcInit( void ) {
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

	sConfig.Channel			=	ADC_CHANNEL_0;
	sConfig.Rank			=	1;
	sConfig.SamplingTime	=	ADC_SAMPLETIME_28CYCLES;

	if ( HAL_ADC_ConfigChannel(&g.adc, &sConfig) != HAL_OK )
		errorHandler();
}


void dacInit ( void ) {
	DAC_ChannelConfTypeDef sConfig;

	g.dac.Instance = DAC;

	if ( HAL_DAC_Init( &g.dac ) != HAL_OK )
		errorHandler();

	sConfig.DAC_Trigger			= DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer	= DAC_OUTPUTBUFFER_ENABLE;

	if ( HAL_DAC_ConfigChannel( &g.dac, &sConfig, DAC_CHANNEL_1 ) != HAL_OK)
		errorHandler();

	if ( HAL_DAC_ConfigChannel( &g.dac, &sConfig, DAC_CHANNEL_2 ) != HAL_OK)
		errorHandler();
}

void timInit ( void ) {
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	g.tim.Instance					=	TIM2;
	g.tim.Init.Prescaler			=	0;
	g.tim.Init.CounterMode			=	TIM_COUNTERMODE_UP;
	g.tim.Init.Period				=	11110;
	g.tim.Init.ClockDivision		=	TIM_CLOCKDIVISION_DIV1;
	g.tim.Init.AutoReloadPreload	=	TIM_AUTORELOAD_PRELOAD_ENABLE;

	if ( HAL_TIM_Base_Init( &g.tim ) != HAL_OK)
		errorHandler();

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if ( HAL_TIM_ConfigClockSource( &g.tim, &sClockSourceConfig) != HAL_OK)
		errorHandler();

	if (HAL_TIM_OC_Init(&g.tim) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&g.tim, &sMasterConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sConfigOC.OCMode = TIM_OCMODE_TIMING;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_OC_ConfigChannel(&g.tim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_TIM_OC_ConfigChannel(&g.tim, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_TIM_OC_ConfigChannel(&g.tim, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_TIM_OC_ConfigChannel(&g.tim, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}
