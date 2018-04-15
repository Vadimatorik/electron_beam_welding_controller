#include "scan_hardware.h"
#include "scan_struct.h"

scanStruct	scan;

void scanStructInit ( void ) {
	scan.curPosCenCor[ 0 ]		= 1.65;
	scan.curPosCenCor[ 1 ]		= 1.65;
	scan.scanAmplitude			= ( uint16_t )( ( float )4096 / ( float )3.3 * ( float )0.3 );
	scan.countAveraging			= 20;
	scan.integrator				= 0.01;
}

extern "C" {

int main(void) {
	scanHardwareInit();
	scanDacArray[ 0 ]->setValue( 2048 );
	scanDacArray[ 1 ]->setValue( 2048 );
	scanAdcObj.startContinuousConversion();
	scanTimInterruptObj.on();
	while( 1 );
}

}
