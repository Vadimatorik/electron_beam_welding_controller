#include "adc.h"

AdcOneChannelCfg adcCfg = {
	.ADCx				=	ADC1,
	.clockPrescaler		=	ADC_CLOCK_SYNC_PCLK_DIV4,
	.resolution			=	ADC_RESOLUTION_12B,
	.dataAlign			=	ADC_DATAALIGN_RIGHT,
	.channel			=	ADC_CHANNEL_0,
	.samplingTime		=	ADC_SAMPLETIME_56CYCLES
};

AdcOneChannel	adcObj( &adcCfg );
