#include "rcc.h"
#include "port.h"
#include "adc.h"

extern Rcc						scanRccObj;
extern GlobalPort				scanGpObj;
extern AdcOneChannel			adcObj;

void mcHardwareInit ( void ) {
	scanRccObj.setCfg( 0 );
	scanGpObj.reinitAllPorts();
	adcObj.reinit();
}

