#include "scan_hardware.h"
#include "stm32f2xx_hal.h"

void scanNvicInit ( void ) {
	HAL_NVIC_SetPriority( TIM2_IRQn, TIM2_HANDLER_PRIO, 0 );
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_NVIC_SetPriority( EXTI2_IRQn, EXTI_HANDLER_PRIO, 0 );
	HAL_NVIC_EnableIRQ( EXTI2_IRQn );
}

void scanHardwareInit ( void ) {
	HAL_Init();

	scanRccObj.setCfg();
	scanGpObj.reinitAllPorts();
	scanAdcObj.reinit();
	scanTimInterruptObj.reinit();
	scanUartObj.reinit();
	scanDacArray[ 0 ]->reinit();
	scanDacArray[ 1 ]->reinit();
	scanNvicInit();
}

