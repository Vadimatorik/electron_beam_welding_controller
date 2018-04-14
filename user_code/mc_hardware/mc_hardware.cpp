#include "rcc.h"

extern rcc scanRccObj;

void mcHardwareInit ( void ) {
	scanRccObj.setCfg( 0 );
}
