#include "main.h"
#include "stm32f2xx_hal.h"
ADC_HandleTypeDef hadc1;
DAC_HandleTypeDef hdac;
TIM_HandleTypeDef htim2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM2_Init(void);

static void MX_USART1_UART_Init(void);
uint32_t adc_cur;

UART_HandleTypeDef huart1;

float ps;

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  HAL_ADC_Start(&hadc1);
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R, 2048 );
  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R, 2048 );

  uint32_t z;

  for ( volatile int l = 0; l < 20; l++ ) {
	  for ( volatile int l1 = 0; l1 < 2000; l1++ ) {};
	  z += adc_cur;
  };

  ps = (float)3.3 / (float)4096 * (z / 20);

  HAL_TIM_Base_Start_IT(&htim2);
  while (1)  {  }
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 20;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/* ADC1 init function */
static void MX_ADC1_Init(void){
  ADC_ChannelConfTypeDef sConfig;

  hadc1.Instance						= ADC1;
  hadc1.Init.ClockPrescaler				= ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution					= ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode				= DISABLE;
  hadc1.Init.ContinuousConvMode			= ENABLE;
  hadc1.Init.DiscontinuousConvMode		= DISABLE;
  hadc1.Init.ExternalTrigConvEdge		= ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv			= ADC_SOFTWARE_START;
  hadc1.Init.DataAlign					= ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion			= 1;
  hadc1.Init.DMAContinuousRequests		= DISABLE;
  hadc1.Init.EOCSelection				= ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
	  _Error_Handler(__FILE__, __LINE__);
  }

}

/* DAC init function */
static void MX_DAC_Init(void)
{

  DAC_ChannelConfTypeDef sConfig;

    /**DAC Initialization 
    */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
	  _Error_Handler(__FILE__, __LINE__);
  }

    /**DAC channel OUT1 config 
    */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
	  _Error_Handler(__FILE__, __LINE__);
  }

    /**DAC channel OUT2 config 
    */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void){
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 11110;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc){
  GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_ADC1_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_ADC_Start_IT(hadc);

   HAL_NVIC_SetPriority(ADC_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}


void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac){
  GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_DAC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

#include <math.h>
uint8_t loop = 0;
uint32_t real_data = 0;
float Xc = 1.6;
uint16_t amplitude = 64;
float test[ 300 ];
uint32_t loop_test = 0;


uint32_t usr_array[20] = { 0 };
uint32_t val_counter[20] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

uint32_t pv = 0;
uint32_t s_2 = 0;



uint8_t flag_new_point = 0;
void TIM2_IRQHandler(void) {
	uint16_t out_pos;
	flag_new_point = 0;
	out_pos = sin( (3.14 * 2) / 20 * loop ) * (float)(amplitude) +  (float)4096 / (float)3.3  * Xc;

	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R, out_pos );
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);

	if ( loop != 0 ) {
		usr_array[loop - 1] = (usr_array[loop - 1] * (val_counter[loop - 1] - 1) + adc_cur) / ((float)val_counter[loop - 1]);
		val_counter[loop - 1]++;
	} else {
		usr_array[19] = (usr_array[19] * (val_counter[19] - 1) + adc_cur) / ((float)val_counter[19]);
		val_counter[19]++;
	}

	loop++;
	if (loop == 20) {
		loop=0;
		s_2++;
	}


	// Экстремум сканирования - 6 (1-й).
	// Экстремум приходящего сигнала - 10.
	// Анализ.
	if (s_2 == 25) {
		s_2 = 0;

		// Убираем постоянную составляющую.
		volatile int32_t p = 0;
		for ( uint32_t l = 0; l < 10; l++) {
			p += usr_array[l];
		}
		for ( uint32_t l = 10; l < 20; l++) {
			p -= usr_array[l];
		}

		if ( p <  0 ) p *= -1;

		int32_t an = usr_array[8] - p;
		uint32_t an_p;
		if ( an <  0 ) an_p = an * -1;
		else an_p = an;

		if ( an_p > 120 ) {		// Мы в стыке.
			if ( an > 0 ) {
				Xc += 0.01;
			} else {
				Xc -= 0.01;
			}
		}

		for ( uint32_t l = 0; l < 20; l++ ) {
					val_counter[ l ] = 1;
					usr_array[ l ] = 0;
				}
	}

}

// Внутренний усреднятор.

uint32_t zp_ob[1020] = { 0 };
uint32_t st = 1;

void ADC_IRQHandler ( void ) {
	uint32_t v = HAL_ADC_GetValue(&hadc1);
	if ( flag_new_point == 0 ) {
		st = 2;
		flag_new_point = 0xFF;
		adc_cur = v;
		return;
	}
	uint32_t adc_cur_buf = (adc_cur * (st - 1) + v) / st;
	st++;
	adc_cur  = adc_cur_buf;

	__HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_STRT | ADC_FLAG_EOC);
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}


float zp[1020] = {0};
volatile uint32_t l_zp = 0;
uint32_t sc = 0;
void EXTI2_IRQHandler(void)
{
	if ( l_zp < 1020 ) {
	zp[l_zp] = Xc;
	l_zp++;
	} else {
		while (l_zp){

		};
	}
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }

}


static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void _Error_Handler(char * file, int line){  while(1)  {  }  }
