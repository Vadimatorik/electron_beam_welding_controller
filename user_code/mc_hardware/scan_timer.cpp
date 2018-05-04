#include "timer.h"
#include "user_os.h"

clkTimBaseCfg scanTimBaseCfg = {
	.period			=	10000,
	.prescaler		=	0,
	.pulse			=	0
};

timInterruptCfg scanTimInterruptCfg = {
	.tim			=	TIM2,
	.cfg			=	&scanTimBaseCfg,
	.countCfg		=	1,
};

TimInterrupt scanTimInterruptObj( &scanTimInterruptCfg );
