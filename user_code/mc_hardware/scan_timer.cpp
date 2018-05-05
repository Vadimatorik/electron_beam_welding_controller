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

clkTimBaseCfg scanModbusTimBaseCfg = {
	.period			=	10416,
	.prescaler		=	0,
	.pulse			=	0
};

timInterruptCfg scanModbusTimInterruptCfg = {
	.tim			=	TIM9,
	.cfg			=	&scanModbusTimBaseCfg,
	.countCfg		=	1,
};

TimInterrupt scanModbusTimInterruptObj( &scanModbusTimInterruptCfg );
