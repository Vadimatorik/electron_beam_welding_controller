#include "dac.h"

DacOneChannelCfg scanDacCh1Cfg = {
	.channel			=	DAC_CHANNEL_1,
	.buffer				=	DAC_OUTPUTBUFFER_DISABLE
};

DacOneChannelCfg scanDacCh2Cfg = {
	.channel			=	DAC_CHANNEL_2,
	.buffer				=	DAC_OUTPUTBUFFER_DISABLE
};

DacOneChannel	scanDac1Obj( &scanDacCh1Cfg );
DacOneChannel	scanDac2Obj( &scanDacCh2Cfg );

DacOneChannelBase*		scanDacArray[ 2 ] = {
	&scanDac1Obj, &scanDac2Obj
};
