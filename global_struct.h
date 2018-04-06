#pragma once

typedef struct {
	ADC_HandleTypeDef			adc;
	DAC_HandleTypeDef			dac;
	TIM_HandleTypeDef			tim;
} globalApplicationStruct;
