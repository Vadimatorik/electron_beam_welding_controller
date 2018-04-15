#include "scan_hardware.h"

uint32_t adc_cur;

float ps;

extern "C" {

int main(void) {
	scanHardwareInit();
	scanDacArray[ 0 ]->setValue( 2048 );
	scanDacArray[ 1 ]->setValue( 2048 );
	scanAdcObj.startContinuousConversion();
	scanTimInterruptObj.on();
	while( 1 );
}

/*
uint8_t loop = 0;
uint32_t real_data = 0;
float Xc = 1.65;
uint16_t amplitude = ( uint16_t )( (double)4096 / (double)3.3 * (double)0.3 );
float test[ 300 ];
uint32_t loop_test = 0;


uint32_t usr_array[20] = { 0 };
uint32_t val_counter[20] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

uint32_t pv = 0;
uint32_t s_2 = 0;



uint8_t flag_new_point = 0;

uint32_t get_pointing ( void ) {
	uint32_t min = 4096;
	uint32_t max = 0;
	uint32_t max_index = 0;
	uint32_t min_index = 0;
	for ( uint32_t i = 0; i < 20; i++ ) {
		if ( usr_array[ i ] < min ) {
			min = usr_array[ i ];
			min_index = i;
		}

		if ( usr_array[ i ] > max ) {
			max = usr_array[ i ];
			max_index = i;
		}
	}
	if ( min_index < max_index ) {
		return 0;
	} else {
		return 1;
	}
}

// Внутренний усреднятор.

uint32_t zp_ob[1020] = { 0 };
uint32_t st = 1;

*/

}
