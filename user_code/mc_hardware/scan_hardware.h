#pragma once

#include "rcc.h"
#include "port.h"
#include "adc.h"
#include "timer.h"
#include "uart.h"
#include "dac.h"

extern Rcc						scanRccObj;
extern GlobalPort				scanGpObj;
extern AdcOneChannel			scanAdcObj;
extern TimInterrupt				scanTimInterruptObj;
extern Uart						scanUartObj;
extern DacOneChannelBase*		scanDacArray[ 2 ];
extern PinMultifuncIt			scanEncoderPinExti;

#define TIM2_HANDLER_PRIO				6
#define EXTI_HANDLER_PRIO				6

void scanHardwareInit ( void );
