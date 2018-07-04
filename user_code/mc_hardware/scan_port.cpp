#include "pin.h"
#include "port.h"
#include "user_os.h"

const pinCfg scanDacPinsCfg = {
	.GPIOx							=	GPIOA,
	.init = {
		.Pin						=	GPIO_PIN_4 | GPIO_PIN_5,
		.Mode						=	GPIO_MODE_ANALOG,
		.Pull						=	GPIO_NOPULL,
		.Speed						=	GPIO_SPEED_FREQ_LOW,
		.Alternate					=	0
	}
};

const pinCfg scanUsart1PinsCfg = {
	.GPIOx							=	GPIOA,
	.init = {
		.Pin						=	GPIO_PIN_9 | GPIO_PIN_10,
		.Mode						=	GPIO_MODE_AF_PP,
		.Pull						=	GPIO_NOPULL,
		.Speed						=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate					=	GPIO_AF7_USART1
	}
};

const pinCfg scanEncoderPinCfg = {
	.GPIOx							=	GPIOA,
	.init = {
		.Pin						=	GPIO_PIN_2,
		.Mode						=	GPIO_MODE_IT_RISING,
		.Pull						=	GPIO_NOPULL,
		.Speed						=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate					=	0
	}
};

const pinCfg scanAdcPinCfg = {
	.GPIOx							=	GPIOA,
	.init = {
		.Pin						=	GPIO_PIN_0,
		.Mode						=	GPIO_MODE_ANALOG,
		.Pull						=	GPIO_NOPULL,
		.Speed						=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate					=	0
	}
};

const pinCfg scanUartDePinCfg = {
	.GPIOx							=	GPIOA,
	.init = {
		.Pin						=	GPIO_PIN_8,
		.Mode						=	GPIO_MODE_OUTPUT_PP,
		.Pull						=	GPIO_NOPULL,
		.Speed						=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate					=	0
	}
};

const pinCfg scanGpCfg[] = {
	scanDacPinsCfg,
	scanUsart1PinsCfg,
	scanEncoderPinCfg,
	scanAdcPinCfg,
	scanUartDePinCfg
};

GlobalPort		scanGpObj( scanGpCfg, M_SIZE_ARRAY( scanGpCfg ) );
PinMultifuncIt	scanEncoderPinExti( &scanEncoderPinCfg, 1, GPIO_PIN_2 );
Pin				scanUartDe( &scanUartDePinCfg );

