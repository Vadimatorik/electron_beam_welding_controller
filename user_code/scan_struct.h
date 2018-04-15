#pragma once

#include "arithmetic_mean.h"

#include <stdint.h>

struct scanStruct {
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

	/// На сколько идет смещение по оси во время сканирования.
	float					integrator;
};
