#pragma once

#ifdef __cplusplus

#include "freertos_headers.h"					// FreeRTOS.
/*
 * Это "усреднятор" float-значений за samples_num значений.
 */
class filtration {
public:
	filtration( float* array, uint32_t samples_num );
	void			addValue		(	float	val	);
	float			getValue		(	void	);

	float			avg_calc		( float val );
	~filtration();

private:
	uint32_t		samples_num;				// Число семплов за которые производится усреднение.
	uint32_t		samples_count		= 0;	// Число семплов от начала работы.
	uint32_t		samples_current		= 0;	// Текущий семпл, куда надо заносить данные.
	float			*buf;
};

#endif
