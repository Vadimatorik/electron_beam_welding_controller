#include "scan_hardware.h"
#include "scan_struct.h"

#include <math.h>

extern scanStruct	scan;
extern uint32_t	getPointing ( void );

extern "C" {

void TIM2_IRQHandler(void) {
	/// Сбрасываем прерывание от таймера.
	scanTimInterruptObj.clearInterruptFlag();

	/// Формируем сигнал сканирования (синусоида вокруг мнимой середины.
	uint16_t outPos;
	outPos =	sin( (3.14 * 2) / 20 * scan.pointAdcMeasurement ) *
				scan.scanAmplitude +
				scan.curPosCenCor[ scan.curAxis ];

	scanDacArray[ scan.curAxis ]->setValue( outPos );

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

	/// Выбираем направление смещения центра оси.
	if ( !getPointing() ) {
		scan.curPosCenCor[ scan.curAxis ] += scan.integrator;
	} else {
		scan.curPosCenCor[ scan.curAxis ] -= scan.integrator;
	}
}

void EXTI2_IRQHandler ( void ) {
	scanEncoderPinExti.clearIt();
}

}

