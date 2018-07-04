#include "scan_hardware.h"
#include "scan_struct.h"
#include "freertos_headers.h"
#include "stm32f2xx_hal.h"

#include <math.h>

extern scanStruct	scan;
extern uint32_t	getPointing ( void );

extern "C" {

const float scanSin[ 20 ] = {	0,				0.308865517,			0.587527514,		0.808736086,
								0.950859487,	0.999999702,			0.951351404,		0.80967176,
								0.58881557,		0.310379922,			0.00159265287,		-0.307350338,
								-0.586238027,	-0.807798266,			-0.950365126,		-0.999997139,
								-0.951840878,	-0.810605466,			-0.590102077,		-0.311893523
};


void tim2_handler(void) {
	/// Сбрасываем прерывание от таймера.
	scanTimInterruptObj.clearInterruptFlag();

	/// Формируем сигнал сканирования (синусоида вокруг мнимой середины.
	float floatOutPos ;
	floatOutPos =	scanSin[ scan.pointAdcMeasurement ] *
					scan.scanAmplitude +
					scan.curPosCenCor[ scan.curAxis ];

	if ( floatOutPos < 0 )	floatOutPos = 0;
	uint16_t outPos = 4096.0 / 3.3 * floatOutPos;

	if ( outPos > 0xFFF )	outPos = 0xFFF;

	scanDacObj.setValue( scan.curAxis, outPos );

	/// Текущее значение с обратной связи в усреднятор.
	uint32_t	adcValue;
	scanAdcObj.getMeasurement( adcValue );
	scan.adcMeasurement[ scan.pointAdcMeasurement ].postVal( static_cast< float >( adcValue ) );

	/// В следующий раз кладем в следующий усреднятор.
	scan.pointAdcMeasurement++;
	if ( scan.pointAdcMeasurement == 20 ) {		/// Если прошли все усредняторы (считали волну), то...
		scan.pointAdcMeasurement = 0;			/// Со следующего элемента кладем с начала, считая, что новая волна.
		scan.nowIterationAveraging++;			/// Ну и запоминаем, что одну волну прошли.
	}

	/// Если просканировали недостаточное количество волн для анализа - выходим.
	if ( scan.nowIterationAveraging != scan.countAveraging ) return;
	scan.nowIterationAveraging = 0;

	/// Выбираем направление смещения центра оси.
	if ( getPointing() ) {
		scan.curPosCenCor[ scan.curAxis ] += scan.integrator;
	} else {
		scan.curPosCenCor[ scan.curAxis ] -= scan.integrator;
	}
}


void setPosAxis ( void ) {
	uint16_t outPos = 4096.0 / 3.3 * scan.axisPos[ scan.encoderTickNow ];
	scanDacObj.setValue( scan.curAxis, outPos );
}

void exti_2_handler ( void ) {
	scanEncoderPinExti.clearIt();

	/// Если идет сканирование.
	if ( scan.flagTypeJob == 0 ) {
		scan.axisPos[ scan.encoderTickNow ] = scan.curPosCenCor[ scan.curAxis ];
		scan.encoderTickNow++;
		if ( scan.encoderTickNow == COUNT_ENCODER_TICK ) {
			scan.encoderTickNow = 0;
			scan.flagTypeJob = 1;
			scanTimInterruptObj.off();
			setPosAxis();
		}
	} else {
		scan.encoderTickNow++;
		setPosAxis();
	}
}

extern void xPortSysTickHandler( void );
void sys_tick_handler () {
	HAL_IncTick();
	xPortSysTickHandler();
}

}

