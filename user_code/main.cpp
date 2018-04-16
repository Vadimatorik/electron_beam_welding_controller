#include "scan_hardware.h"
#include "scan_struct.h"

scanStruct	scan;

void scanStructInit ( void ) {
	scan.curPosCenCor[ 0 ]		= 1.65;
	scan.curPosCenCor[ 1 ]		= 1.65;
	scan.scanAmplitude			= 0.3;
	scan.countAveraging			= 20;
	scan.integrator				= 0.04;
}

extern "C" {

int main(void) {
	scanHardwareInit();
	scanStructInit();
	scanDacObj.setValue( 0, 2048 );
	scanDacObj.setValue( 1, 2048 );
	scanAdcObj.startContinuousConversion();
	scanTimInterruptObj.on();
	while( 1 );
}

}
