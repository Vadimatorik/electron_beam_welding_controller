#include "scan_hardware.h"
#include "stm32f2xx_hal.h"

void scanNvicInit ( void ) {
	HAL_NVIC_SetPriority( TIM2_IRQn, TIM2_HANDLER_PRIO, 0 );
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_NVIC_SetPriority( EXTI2_IRQn, EXTI_HANDLER_PRIO, 1 );
	HAL_NVIC_EnableIRQ( EXTI2_IRQn );

	HAL_NVIC_SetPriority( EXTI3_IRQn, EXTI_HANDLER_PRIO, 1 );
	HAL_NVIC_EnableIRQ( EXTI3_IRQn );

	HAL_NVIC_SetPriority( USART1_IRQn, USART1_HANDLER_PRIO, 2 );
	HAL_NVIC_EnableIRQ( USART1_IRQn );

	HAL_NVIC_SetPriority( TIM1_BRK_TIM9_IRQn, TIM9_HANDLER_PRIO, 3 );
	HAL_NVIC_EnableIRQ( TIM1_BRK_TIM9_IRQn );
}

void scanHardwareInit ( void ) {
	HAL_Init();

	scanRccObj.setCfg();
	scanGpObj.reinitAllPorts();
	scanAdcObj.reinit();
	scanTimInterruptObj.reinit();
	scanModbusTimInterruptObj.reinit();
	scanUartObj.reinit();
	scanDacObj.reinit();

	scanNvicInit();
}

