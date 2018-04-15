#include "scan_struct.h"

extern scanStruct	scan;

uint32_t getPointing ( void ) {
	float min = 5000;
	float max = 0;
	uint32_t max_index = 0;
	uint32_t min_index = 0;

	for ( uint32_t i = 0; i < 20; i++ ) {
		float valueBuffer = scan.adcMeasurement[ i ].getAndReset();

		if ( valueBuffer < min ) {
			min = valueBuffer;
			min_index = i;
		}

		if ( valueBuffer > max ) {
			max = valueBuffer;
			max_index = i;
		}
	}
	if ( min_index < max_index ) {
		return 0;
	} else {
		return 1;
	}
}
