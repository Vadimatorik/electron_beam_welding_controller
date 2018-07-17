#include "scan_hardware.h"
#include "scan_struct.h"

scanStruct	scan;

#include "dac.h"

void scanStructInit ( void ) {
	scan.curPosCenCor[ 0 ]		= 1.65;
	scan.curPosCenCor[ 1 ]		= 1.65;
	scan.scanAmplitude			= 0.3;
	scan.countAveraging			= 20;
	scan.integrator				= 0.04;
}

#include <math.h>

int main( void ) {
	scanHardwareInit();
	scanStructInit();
	scanDacObj.setValue( 0, 2048 );
	scanDacObj.setValue( 1, 2048 );
	scanAdcObj.startContinuousConversion();
	scanTimInterruptObj.on();

	modbusInit();
	ModBusRTU_Slave_Init_Addr_Speed( &scan.mb.ModBusRTU_Slave, 0x02, 0 );

	scan.mb.RegMap_Table_1[0] = 0x33;

	for ( int i = 1; i < 513; i++ ) {
		scan.mb.RegMap_Table_1[ i ] = sin( static_cast< double >( i - 1 ) * 3.14 / 512 * 2 ) * ( 4096 / 2 ) + 4096 / 2;
	}

	while( 1 ) {
		ModBusRTU_Slave_Service( &scan.mb.ModBusRTU_Slave );
	}
}
