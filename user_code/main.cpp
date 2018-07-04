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


int main( void ) {
	scanHardwareInit();
	scanStructInit();
	scanDacObj.setValue( 0, 2048 );
	scanDacObj.setValue( 1, 2048 );
	scanAdcObj.startContinuousConversion();
	scanTimInterruptObj.on();

	modbusInit();
	ModBusRTU_Slave_Init_Addr_Speed( &scan.mb.ModBusRTU_Slave, 0x01, 0 );

	scan.mb.RegMap_Table_1[0] = 0x01;
	scan.mb.RegMap_Table_1[1] = 0x02;
	scan.mb.RegMap_Table_1[2] = 0x03;
	scan.mb.RegMap_Table_1[3] = 0x04;
	scan.mb.RegMap_Table_1[4] = 0x05;


	while( 1 ) {
		ModBusRTU_Slave_Service( &scan.mb.ModBusRTU_Slave );
	}
}
