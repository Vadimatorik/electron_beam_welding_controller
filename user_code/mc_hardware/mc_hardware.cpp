#include "rcc.h"
#include "port.h"

extern Rcc						scanRccObj;
extern GlobalPort				scanGpObj;

void mcHardwareInit ( void ) {
	scanRccObj.setCfg( 0 );
	scanGpObj.reinitAllPorts();
}

