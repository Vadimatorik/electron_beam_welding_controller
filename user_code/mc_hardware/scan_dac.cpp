#include "dac.h"
#include "user_os.h"

DacCfg scanDacCfg[] = {
	{
		.buffer				=	DAC_OUTPUTBUFFER_DISABLE,
		.defaultValue		=	2048
	}
};

Dac	scanDacObj( scanDacCfg, M_SIZE_ARRAY( scanDacCfg ) );
