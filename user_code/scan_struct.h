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

	/*!
	 * Мы усредняем измерения по 20 точкам циклично.
	 * Иначе говоря, мы точно знаем, что примерно
	 * синусоида состоит из 20 точек. И мы собираем
	 * кучу таких "типа синусоид", а потом смотрим
	 * на что-то действительно похожее на синусоиду.
	 */
	arithmeticMean			adcMeasurement[ 20 ];
	uint32_t				pointAdcMeasurement;

	/// Амплитуда сканирования в вольтах.
	float					scanAmplitude;

	/// Количество итераций усреднения входной синусоиды с датчика.
	uint32_t				countAveraging;

	/// Текущий этап усреднения.
	uint32_t				nowIterationAveraging;

	/// Массив точек энкодера на активной оси.
	float					axisPos[ COUNT_ENCODER_TICK ];



	/// ModBus.
	scanModbus				mb;
};
