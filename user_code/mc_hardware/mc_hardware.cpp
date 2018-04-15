#include "rcc.h"
#include "port.h"
#include "adc.h"
#include "timer.h"

extern Rcc						scanRccObj;
extern GlobalPort				scanGpObj;
extern AdcOneChannel			scanAdcObj;
extern TimInterrupt				scanTimInterruptObj;

void mcHardwareInit ( void ) {
	scanRccObj.setCfg();
	scanGpObj.reinitAllPorts();
	scanAdcObj.reinit();
	scanTimInterruptObj.reinit();
}

