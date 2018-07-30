#pragma once

#include "arithmetic_mean.h"
#include "scan_modbus.h"
#include <stdint.h>

#define COUNT_ENCODER_TICK						50

struct scanStruct {

	/// 0 - ничего не происходит.
	/// 1 - начато сканирование.
	/// 2 - закончено сканирование и идет воспроизведение.
	uint32_t				state;

	/// Ось, относительно которой сейчас идет управление (0 или 1).
	uint32_t				curAxis;

	/// Текущее положение центра оси X и Y координат.
	float					curPosCenCor[2];

	uint32_t				pointAdcMeasurementNow;
	int32_t					pos;

	uint32_t				posAnal;


	/// Количество итераций усреднения входной синусоиды с датчика.
	uint32_t				countAveraging;

	/// Текущий этап усреднения.
	uint32_t				nowIterationAveraging;

	/// Массив точек энкодера на активной оси.
	float					axisPos[ COUNT_ENCODER_TICK ];



	/// ModBus.
	scanModbus				mb;
};
