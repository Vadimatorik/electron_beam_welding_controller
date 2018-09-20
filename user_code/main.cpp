#include "scan_hardware.h"
#include "scan_struct.h"

scanStruct				scan;
float filterBuffer[ 50 ] = { 0 };
filtration				filter( filterBuffer, 50 );

#include "dac.h"

void scanStructInit ( void ) {
	scan.curPosCenCor[ 0 ]		=	1.65;
	scan.curPosCenCor[ 1 ]		=	1.65;
	scan.countAveraging			=	20;
	scan.state					=	0;
	scan.posAnal				=	6;
}

#include <math.h>

int main( void ) {
	scanHardwareInit();
	scanStructInit();
	scanDacObj.setValue( 0, 2048 );
	scanDacObj.setValue( 1, 2048 );
	scanAdcObj.startContinuousConversion();

	modbusInit();
	ModBusRTU_Slave_Init_Addr_Speed( &scan.mb.ModBusRTU_Slave, 0x02, 0 );

	scan.mb.RegMap_Table_1[0] = 0;

	for ( int i = 1; i < 513; i++ ) {
		scan.mb.RegMap_Table_1[ i ] = 0;
	}
	
	while( 1 ) {
		ModBusRTU_Slave_Service( &scan.mb.ModBusRTU_Slave );

		if ( ( scan.mb.RegMap_Table_1[0] & ( 1 << 13 ) ) != 0 ) {
			if ( scan.state == 0 ) {
				scan.state	=	1;
				/// Сброс энкодера.
				scan.mb.RegMap_Table_1[ 0 ]	&=	~0b111111111;

				scan.curAxis	=	( scan.mb.RegMap_Table_1[0] & ( 1 << 12 ) ) >> 12;

				scan.pid.kp		=	scan.mb.RegMap_Table_1[ 515 ] / 10000.0;
				scan.pid.ki		=	scan.mb.RegMap_Table_1[ 516 ] / 10000.0;
				scan.pid.kd		=	scan.mb.RegMap_Table_1[ 517 ] / 10000.0;

				scan.pid.iMax	=	scan.mb.RegMap_Table_1[ 519 ] / 10000.0;
				scan.pid.iMin	=	( scan.mb.RegMap_Table_1[ 519 ] / 10000.0 ) * -1.0;

				scan.pid.outMax	=	( scan.mb.RegMap_Table_1[ 520 ] / 10000.0 );
				scan.pid.outMin	=	( scan.mb.RegMap_Table_1[ 520 ] / 10000.0 ) * -1.0;

				filter.reset();

				scanTimInterruptObj.on();
			}
		}

		


	}
}
