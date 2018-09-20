#include "filtration_cpp.h"

filtration::filtration( float* array, uint32_t samples_num ):
	samples_num(samples_num) {
	this->buf = array;
	configASSERT ( this->buf );
}

filtration::~filtration() {
	delete this->buf;
}

void filtration::reset ( void ) {
	this->samples_count		=	0;
	this->samples_current	=	0;
}

void filtration::addValue (	float	val	) {
	this->buf[this->samples_current] = val;
		this->samples_current++;

		if ( this->samples_current >= this->samples_num ) {
			this->samples_current = 0;
		}
		if ( this->samples_count < this->samples_num ) {
			this->samples_count++;
		}
}

float filtration::getValue ( void ) {
	float avg = 0;

	for (uint32_t i=0; i<this->samples_count; i++) {
		avg +=  this->buf[i];
	}

	return avg / this->samples_count;
}


float filtration::avg_calc ( float val ) {
	// Заносим значение.
	this->buf[this->samples_current] = val;
	this->samples_current++;

	if ( this->samples_current >= this->samples_num ) {
		this->samples_current = 0;
	}
	if ( this->samples_count < this->samples_num ) {
		this->samples_count++;
	}

	float avg = 0;

	// Вычисляем среднее.
	for (uint32_t i=0; i<this->samples_count; i++) {
		avg +=  this->buf[i];
	}

	return avg / this->samples_count;
}
