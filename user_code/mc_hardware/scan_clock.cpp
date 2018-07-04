#include "rcc.h"
#include "user_os.h"

const rccCfg scanRccCfgArray[] = {
	/*!
	 * Индекс: 0
	 * Источник: внешний резонатор на 25 МГц.
	 * PCLK:			120 МГц.
	 * AHB1:			120 МГц.
	 * APB2:			60 МГц.
	 * APB1:			30 МГц.
	 */
	{
		.osc = {
			.OscillatorType						= RCC_OSCILLATORTYPE_HSE,
			.HSEState							= RCC_HSE_ON,
			.LSEState							= RCC_LSE_OFF,
			.HSIState							= RCC_HSI_OFF,
			.HSICalibrationValue				= RCC_HSICALIBRATION_DEFAULT,
			.LSIState							= RCC_LSI_OFF,
			.PLL = {
				.PLLState						= RCC_PLL_ON,
				.PLLSource						= RCC_PLLSOURCE_HSE,
				.PLLM							= 20,
				.PLLN							= 192,
				.PLLP							= RCC_PLLP_DIV2,
				.PLLQ							= 4
		   	}
		},
		.clk = {
			.ClockType							= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
			.SYSCLKSource						= RCC_SYSCLKSOURCE_PLLCLK,
			.AHBCLKDivider						= RCC_SYSCLK_DIV1,
			.APB1CLKDivider						= RCC_HCLK_DIV4,
			.APB2CLKDivider						= RCC_HCLK_DIV2
		},
		.fLatency								= FLASH_LATENCY_3
	}
};

Rcc scanRccObj( scanRccCfgArray, M_SIZE_ARRAY( scanRccCfgArray ) );
