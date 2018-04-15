#include "timer.h"

clkTimBaseCfg scanTimBaseCfg = {
	.period			=	10000,
	.prescaler		=	0,
	.pulse			=	0
};

timInterruptCfg scanTimInterruptCfg = {
	.tim			=	TIM2,
	.timChannel		=	HAL_TIM_ACTIVE_CHANNEL_1,
	.cfg			=	&scanTimBaseCfg,
	.countCfg		=	1,
};

TimInterrupt scanTimInterruptObj( &scanTimInterruptCfg );
