#include "dac.h"

DacCfg scanDacCfg = {
	.buffer				=	DAC_OUTPUTBUFFER_DISABLE
};

Dac	scanDacObj( &scanDacCfg );
