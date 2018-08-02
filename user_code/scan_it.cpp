#include "scan_hardware.h"
#include "scan_struct.h"
#include "stm32f4xx_hal.h"
#include "stm32f415xx.h"
#include "scan_modbus.h"

#include <math.h>

extern Uart				scanUartObj;
extern TimInterrupt		scanModbusTimInterruptObj;
extern scanStruct	scan;

extern PinMultifuncIt	scanEncoderPin_1_Exti;
extern PinMultifuncIt	scanEncoderPin_2_Exti;

extern uint32_t	getPointing ( void );

extern "C" {

float scanSin[ 20 ] = {		0,				0.308865517,			0.587527514,		0.808736086,
							0.950859487,		0.999999702,			0.951351404,		0.80967176,
							0.58881557,		0.310379922,			0.00159265287,		-0.307350338,
							-0.586238027,		-0.807798266,			-0.950365126,		-0.999997139,
							-0.951840878,	-	0.810605466,			-0.590102077,		-0.311893523
};

struct pointAdc {
	uint32_t		index;
	uint32_t		value;
};

__attribute__ ((section ("data")))
static pointAdc min;

__attribute__ ((section ("data")))
static pointAdc max;

#define PERIOD_PID					1.0 / 300.0

__attribute__ ((section (".ramfunc")))
float pid_update ( float val ) {
	float out;

	static float				integrator	= 0;
	static float				prev_in		= 0;

	integrator += val * scan.pid.ki * PERIOD_PID;
	if ( integrator > scan.pid.iMax ) {
		integrator = scan.pid.iMax;
	}
	if ( integrator < scan.pid.iMin ) {
		integrator = scan.pid.iMin;
	}
	if ( std::isnan(integrator) || std::isinf(integrator) ) {
		//некорректная ситуация! Сбрасываем в ноль в надежде восстановить работу
		integrator = 0;
	}
	out = val * scan.pid.kp + integrator + (val - prev_in)*scan.pid.kd/PERIOD_PID;

	prev_in = val;

	out = out > scan.pid.outMax ? scan.pid.outMax : out;
	out = out < scan.pid.outMin ? scan.pid.outMin : out;
	return out;
}

__attribute__ ((section (".ramfunc")))
void TIM2_IRQHandler ( void ) {
	/// Сбрасываем прерывание от таймера.
	TIM2->SR = 0;

	/// Формируем сигнал сканирования (синусоида вокруг мнимой середины.
	float floatOutPos ;
	floatOutPos =	scanSin[ scan.pointAdcMeasurementNow ] *
					scan.mb.RegMap_Table_1[ 514 ] * 1.0 / 100.0 +
					scan.curPosCenCor[ scan.curAxis ];

	if ( floatOutPos < 0 )	floatOutPos = 0;
	uint16_t outPos = 4096.0 / 3.3 * floatOutPos;

	if ( outPos > 0xFFF )	outPos = 0xFFF;

	if ( !scan.curAxis ) {
		DAC1->DHR12R1	=	outPos;
	} else {
		DAC1->DHR12R2	=	outPos;
	}

	/// Текущее значение с обратной связи в усреднятор.
	uint32_t	adcValue;
	adcValue	=	ADC1->DR;

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

	float error = ( max.value - min.value ) * 3.3 / 4096.0;

	if ( scan.mb.RegMap_Table_1[ 513 ] ) {
		if ( min.index > max.index  ) {
			error *= -1;
		}
	} else {
		if ( min.index < max.index  ) {
			error *= -1;
		}
	}

	error *= scan.mb.RegMap_Table_1[ 518 ] / 10000.0;

	scan.curPosCenCor[ scan.curAxis ] += pid_update( error );
}

static int psevDoEnc = 0;

__attribute__ ((section (".ramfunc")))
void inc_encoder ( void ) {
	uint32_t	encoderTick;
	encoderTick	=	scan.mb.RegMap_Table_1[ 0 ];
	encoderTick	&=	0b111111111;

	if ( scan.state == 1 ) {
		scan.mb.RegMap_Table_1[ 0 ]	&=	~0b111111111;
		scan.mb.RegMap_Table_1[ 0 ]	|=	encoderTick;
		scan.mb.RegMap_Table_1[ encoderTick + 1 ]	=	4096.0 / 3.3 * scan.curPosCenCor[ scan.curAxis ];
	} else {
			if ( !scan.curAxis ) {
					DAC1->DHR12R1	=	scan.mb.RegMap_Table_1[ encoderTick + 1 ];
				} else {
					DAC1->DHR12R2	=	scan.mb.RegMap_Table_1[ encoderTick + 1 ];
				}
		}

	encoderTick	+=	1;
	encoderTick	&=	0b111111111;

	scan.mb.RegMap_Table_1[ 0 ] &= ~0b111111111;
	scan.mb.RegMap_Table_1[ 0 ] |= encoderTick;

	if ( encoderTick == 0 ) {
		if ( scan.state == 1 ) {
			scan.state = 0;
			scan.mb.RegMap_Table_1[0] &= ~( 1 << 13 );
		}
	}
}



#define ENCODER_POINTS			500
__attribute__ ((section (".ramfunc")))
void dec_encoder ( void ) {
	int32_t	encoderTick;
	encoderTick	=	scan.mb.RegMap_Table_1[ 0 ];
	encoderTick	&=	0b111111111;

	if ( encoderTick < 0 ) {
		encoderTick	=	ENCODER_POINTS - 1;
		if ( scan.state == 1 ) {
			scan.state = 0;
			scan.mb.RegMap_Table_1[0] &= ~( 1 << 13 );
			scanTimInterruptObj.off();
		}
	}

	encoderTick	-=	1;
	encoderTick	&=	0b111111111;

	scan.mb.RegMap_Table_1[ 0 ] &= ~0b111111111;
		scan.mb.RegMap_Table_1[ 0 ] |= encoderTick;

	if ( scan.state == 1 ) {
		scan.mb.RegMap_Table_1[ 0 ]	&=	~0b111111111;
		scan.mb.RegMap_Table_1[ 0 ]	|=	encoderTick;
		scan.mb.RegMap_Table_1[ encoderTick + 1 ]	=	scan.curPosCenCor[ scan.curAxis ];
	} else {
		if ( !scan.curAxis ) {
				DAC1->DHR12R1	=	scan.mb.RegMap_Table_1[ encoderTick + 1 ];
			} else {
				DAC1->DHR12R2	=	scan.mb.RegMap_Table_1[ encoderTick + 1 ];
			}
	}
}


__attribute__ ((section (".ramfunc")))
void EXTI2_IRQHandler ( void ) {
	uint32_t reg = GPIOA->IDR;
	__HAL_GPIO_EXTI_CLEAR_IT( GPIO_PIN_2 );

	if ( ( ( reg & ( 1 << 3 ) ) != 0 ) ) {
		psevDoEnc--;
		if ( psevDoEnc < 0 ) {
			psevDoEnc = 17;
			dec_encoder();
		}
	} else {
		psevDoEnc++;
		if ( psevDoEnc > 17 ) {
			psevDoEnc = 0;
			inc_encoder();
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

void CRYP_IRQHandler				( void ) {
	NVIC_SystemReset();
}

void FPU_IRQHandler					( void ) {
	NVIC_SystemReset();
}


}

