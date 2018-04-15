#include "scan_hardware.h"
#include <math.h>

extern "C" {

void TIM2_IRQHandler(void) {
	/*
	uint16_t outPos;

	outPos = sin( (3.14 * 2) / 20 * loop ) * (float)(amplitude) +  (float)4096 / (float)3.3  * Xc;

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
		if (s_2 == 20) {
			s_2 = 0;

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
*/
/*
	void ADC_IRQHandler ( void ) {

		uint32_t v;
		scanAdcObj.getMeasurement( v );
		if ( flag_new_point == 0 ) {
			st = 2;
			flag_new_point = 0xFF;
			adc_cur = v;
			return;
		}
		uint32_t adc_cur_buf = (adc_cur * (st - 1) + v) / st;
		st++;
		adc_cur  = adc_cur_buf;


		scanAdcObj.irqHandler();
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
	*/
}

}
