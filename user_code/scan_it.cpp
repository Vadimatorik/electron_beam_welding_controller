#include "scan_hardware.h"
#include "scan_struct.h"
#include "freertos_headers.h"
#include "stm32f2xx_hal.h"
#include "stm32f205xx.h"
#include "scan_modbus.h"

#include <math.h>

extern Uart				scanUartObj;
extern TimInterrupt		scanModbusTimInterruptObj;
extern scanStruct	scan;

extern PinMultifuncIt	scanEncoderPin_1_Exti;
extern PinMultifuncIt	scanEncoderPin_2_Exti;

extern uint32_t	getPointing ( void );

extern "C" {

const float scanSin[ 20 ] = {	0,				0.308865517,			0.587527514,		0.808736086,
								0.950859487,	0.999999702,			0.951351404,		0.80967176,
								0.58881557,		0.310379922,			0.00159265287,		-0.307350338,
								-0.586238027,	-0.807798266,			-0.950365126,		-0.999997139,
								-0.951840878,	-0.810605466,			-0.590102077,		-0.311893523
};

struct pointAdc {
	uint32_t		index;
	uint32_t		value;
};

void TIM2_IRQHandler ( void ) {
	/// Сбрасываем прерывание от таймера.
	scanTimInterruptObj.clearInterruptFlag();

	/// Формируем сигнал сканирования (синусоида вокруг мнимой середины.
	float floatOutPos ;
	floatOutPos =	scanSin[ scan.pointAdcMeasurementNow ] *
					scan.mb.RegMap_Table_1[ 514 ] * 1.0 / 100.0 +
					scan.curPosCenCor[ scan.curAxis ];

	if ( floatOutPos < 0 )	floatOutPos = 0;
	uint16_t outPos = 4096.0 / 3.3 * floatOutPos;

	if ( outPos > 0xFFF )	outPos = 0xFFF;

	scanDacObj.setValue( scan.curAxis, outPos );

	/// Текущее значение с обратной связи в усреднятор.
	uint32_t	adcValue;
	adcValue	=	scanAdcObj.getMeasurement();

	static pointAdc min;
	static pointAdc max;

	if ( scan.pointAdcMeasurementNow == 0 ) {
		min.value = adcValue;
		max.value = adcValue;
	}

	if ( adcValue < min.value ) {
		min.value = adcValue;
		min.index = scan.pointAdcMeasurementNow;
	}

	if ( adcValue > max.value ) {
		max.value = adcValue;
		max.index = scan.pointAdcMeasurementNow;
	}

	if ( scan.pointAdcMeasurementNow == scan.posAnal ) {
		scan.pos = adcValue;
	}

	scan.pointAdcMeasurementNow++;

	if ( scan.pointAdcMeasurementNow != 20)
		return;

	scan.pointAdcMeasurementNow = 0;

	if ( min.index > max.index ) {
		scan.curPosCenCor[ scan.curAxis ] += scan.mb.RegMap_Table_1[ 513 ] * 3.3 / 4096;
	} else {
		scan.curPosCenCor[ scan.curAxis ] -= scan.mb.RegMap_Table_1[ 513 ] * 3.3 / 4096;
	}
}


void setPosAxis ( void ) {
	//uint16_t outPos = 4096.0 / 3.3 * scan.axisPos[ scan.encoderTickNow ];
	//scanDacObj.setValue( scan.curAxis, outPos );
}
///	if ( ( ( GPIOA->ODR & ( 1 << 2 ) ) == 0 ) ) {

/*
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
	}*/

__attribute__ ((section (".ramfunc")))
void EXTI2_IRQHandler ( void ) {
	__HAL_GPIO_EXTI_CLEAR_IT( GPIO_PIN_2 );

	if ( ( ( GPIOA->IDR & ( 1 << 3 ) ) == 0 ) ) {
		int32_t	encoderTick;
		encoderTick	=	scan.mb.RegMap_Table_1[ 0 ];
		encoderTick	&=	0b111111111;
		encoderTick	-=	1;

		if ( encoderTick < 0 ) {
			encoderTick	=	511;
			if ( scan.state == 1 ) {
				scan.state = 0;
				scan.mb.RegMap_Table_1[0] &= ~( 1 << 13 );
			}
		}

		if ( scan.state == 1 ) {
			scan.mb.RegMap_Table_1[ 0 ]	&=	~0b111111111;
			scan.mb.RegMap_Table_1[ 0 ]	|=	encoderTick;

			scan.mb.RegMap_Table_1[ encoderTick + 1 ]	=	scan.curPosCenCor[ scan.curAxis ];
		}
	}

}

#include <cmath>

__attribute__ ((section (".ramfunc")))
void EXTI3_IRQHandler ( void ) {
	__HAL_GPIO_EXTI_CLEAR_IT( GPIO_PIN_3 );

	if ( ( ( GPIOA->IDR & ( 1 << 2 ) ) == 0 ) ) {
		uint32_t	encoderTick;
		encoderTick	=	scan.mb.RegMap_Table_1[ 0 ];
		encoderTick	&=	0b111111111;
		encoderTick	+=	1;

		if ( encoderTick == 512 ) {
			encoderTick	=	0;
			if ( scan.state == 1 ) {
							scan.state = 0;
							scan.mb.RegMap_Table_1[0] &= ~( 1 << 13 );
						}
		}

		if ( scan.state == 1 ) {
			scan.mb.RegMap_Table_1[ 0 ]	&=	~0b111111111;
			scan.mb.RegMap_Table_1[ 0 ]	|=	encoderTick;

			scan.mb.RegMap_Table_1[ encoderTick + 1 ]	=	4096.0 / 3.3 * scan.curPosCenCor[ scan.curAxis ];
		}
	}
}

extern void xPortSysTickHandler( void );
void SysTick_Handler () {
	HAL_IncTick();
	xPortSysTickHandler();
}

void TIM1_BRK_TIM9_IRQHandler ( void ) {
	ModBusRTU_Slave_TimerTic( &scan.mb.ModBusRTU_Slave );
	scanModbusTimInterruptObj.clearInterruptFlag();
}

#define USART1_GET_IT_FLAG(FLAG) ((USART1->CR1 & (FLAG)) == (FLAG))
#define USART1_GET_FLAG(FLAG) ((USART1->SR & (FLAG)) == (FLAG))


void USART1_IRQHandler ( void ) {
	if ( USART1_GET_IT_FLAG(USART_CR1_TXEIE) ) {
		if ( USART1_GET_FLAG( UART_FLAG_TXE ) ) {
			ModBusRTU_Slave_InterBytes_Sent( &scan.mb.ModBusRTU_Slave );
		}
	}

	if ( USART1_GET_IT_FLAG(USART_CR1_RXNEIE) ) {
		if ( USART1_GET_FLAG( UART_FLAG_RXNE ) ) {
			ModBusRTU_Slave_Byte_Read( &scan.mb.ModBusRTU_Slave, USART1->DR );
		}
	}

	if ( USART1_GET_FLAG( UART_FLAG_ORE | UART_FLAG_NE | UART_FLAG_FE | UART_FLAG_PE ) ) {
		volatile  uint32_t reg;
		reg = USART1->SR;
		reg = USART1->DR;
		( void )reg;
	}
}




}

