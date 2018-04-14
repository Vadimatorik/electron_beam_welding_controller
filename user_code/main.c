#include "stm32f2xx_hal.h"
#include "hardware_init.h"
#include "global_struct.h"

globalApplicationStruct		g = { 0 };

uint32_t adc_cur;

float ps;

int main(void) {
	hardwareInit();

  HAL_DAC_Start( &g.dac, DAC_CHANNEL_1 );
  HAL_DAC_Start( &g.dac, DAC_CHANNEL_2 );
  HAL_DAC_SetValue(&g.dac,DAC_CHANNEL_1,DAC_ALIGN_12B_R, 2048 );
  HAL_DAC_SetValue(&g.dac,DAC_CHANNEL_2,DAC_ALIGN_12B_R, 2048 );

  HAL_ADC_Start_IT( &g.adc );

 //uint32_t z;

  /*
  for ( volatile int l = 0; l < 20; l++ ) {
	  for ( volatile int l1 = 0; l1 < 2000; l1++ ) {};
	  z += adc_cur;
  };*/

  //ps = (float)3.3 / (float)4096 * (z / 20);

  HAL_TIM_Base_Start_IT(&g.tim);
  while (1)  {  }
}

#include <math.h>
uint8_t loop = 0;
uint32_t real_data = 0;
float Xc = 1.65;
uint16_t amplitude = ( uint16_t )( (double)4096 / (double)3.3 * (double)0.3 );
float test[ 300 ];
uint32_t loop_test = 0;


uint32_t usr_array[20] = { 0 };
uint32_t val_counter[20] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

uint32_t pv = 0;
uint32_t s_2 = 0;



uint8_t flag_new_point = 0;

uint32_t get_pointing ( void ) {
	uint32_t min = 4096;
	uint32_t max = 0;
	uint32_t max_index = 0;
	uint32_t min_index = 0;
	for ( uint32_t i = 0; i < 20; i++ ) {
		if ( usr_array[ i ] < min ) {
			min = usr_array[ i ];
			min_index = i;
		}

		if ( usr_array[ i ] > max ) {
			max = usr_array[ i ];
			max_index = i;
		}
	}
	if ( min_index < max_index ) {
		return 0;
	} else {
		return 1;
	}
}


void TIM2_IRQHandler(void) {
	volatile uint16_t out_pos;
	flag_new_point = 0;
	out_pos = sin( (3.14 * 2) / 20 * loop ) * (float)(amplitude) +  (float)4096 / (float)3.3  * Xc;

	HAL_DAC_SetValue(&g.dac,DAC_CHANNEL_1,DAC_ALIGN_12B_R, out_pos );
	__HAL_TIM_CLEAR_IT(&g.tim, TIM_IT_UPDATE);

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
	if (s_2 == 20/* 25*/) {
		s_2 = 0;
/*
		// Убираем постоянную составляющую.
		volatile int32_t p = 0;
		for ( uint32_t l = 0; l < 10; l++) {
			p += usr_array[l];
		}
		for ( uint32_t l = 10; l < 20; l++) {
			p -= usr_array[l];
		}

		if ( p <  0 ) p *= -1;

		int32_t an = usr_array[5] - p;
*/
			if ( !get_pointing() ) {
				Xc += 0.01;
			} else {
				Xc -= 0.01;
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
	uint32_t v = HAL_ADC_GetValue(&g.adc);
	if ( flag_new_point == 0 ) {
		st = 2;
		flag_new_point = 0xFF;
		adc_cur = v;
		return;
	}
	uint32_t adc_cur_buf = (adc_cur * (st - 1) + v) / st;
	st++;
	adc_cur  = adc_cur_buf;

	__HAL_ADC_CLEAR_FLAG( &g.adc, ADC_FLAG_STRT | ADC_FLAG_EOC );
}

float zp[1020] = {0};
volatile uint32_t l_zp = 0;
uint32_t sc = 0;
void EXTI2_IRQHandler(void)
{
	/*
	if ( l_zp < 1020 ) {
	zp[l_zp] = Xc;
	l_zp++;
	} else {
		while (l_zp){

		};
	}*/
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void _Error_Handler(char * file, int line){  while(1)  {  }  }

void errorHandler ( void ) {
	while( 1 ) {

	}
}
