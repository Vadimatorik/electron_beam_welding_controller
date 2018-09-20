#pragma once

#include "arithmetic_mean.h"
#include "scan_modbus.h"
#include <stdint.h>
#include "filtration_cpp.h"

#define COUNT_ENCODER_TICK		50
#define PERIOD_PID				1.0 / 300.0
#define ENCODER_POINTS			500
#define	SV_START				(1 << 15)

struct pidSyscfg {
	float kp;
	float ki;
	float kd;
	float iMax;
	float iMin;
	float outMax;
	float outMin;
};

struct scanStruct {

	/// 0 - ничего не происходит.
	/// 1 - начато сканирование.
	/// 2 - закончено сканирование и идет воспроизведение.
	uint32_t				state;

	/// Ось, относительно которой сейчас идет управление (0 или 1).
	uint32_t				curAxis;

	/// Текущее положение центра оси X и Y координат.
	float					curPosCenCor[2];

	float					curPosCenCorFilter[2];

	uint32_t				pointAdcMeasurementNow;
	int32_t					pos;

	uint32_t				posAnal;


	/// Количество итераций усреднения входной синусоиды с датчика.
	uint32_t				countAveraging;

	/// Текущий этап усреднения.
	uint32_t				nowIterationAveraging;

	/// ModBus.
	scanModbus				mb;

	pidSyscfg				pid;

	uint32_t				downCounterEncoder;
};
