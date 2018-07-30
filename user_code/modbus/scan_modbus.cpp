#include "scan_modbus.h"
#include "scan_struct.h"
#include "scan_modbus_low_lavel.h"

extern scanStruct	scan;

void modbusInit ( void ) {
	// Инициализировать таблицу регистров #1
	ModBus_Slave_Init_Registers_Table(	&scan.mb.ModBusRTU_Slave_RegistersTable[0],
										scan.mb.RegMap_Table_1,
										ACCESS_REG_RW,
										0,
										1 + 512	+ 1 + 1 );

	// Инициализирует карту таблиц регистров
	ModBus_Slave_Init_Registers_Map_Table(	&scan.mb.ModBusRTU_Slave_RegistersMapTable,
											scan.mb.ModBusRTU_Slave_RegistersTable,
											1	);


	// Инициализация ModBusRTU_Slave
	ModBusRTU_Slave_Init(	&scan.mb.ModBusRTU_Slave,
							&scan.mb.ModBusRTU_Slave_RegistersMapTable,
			scan.mb.ModBusRTU_Slave_RxTxBuff);


	/// Указываем все требуемые функции обратных вызовов.
	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Receiv_Phisic =
			modBusRtuSlaveDisableInterReceivPhisic;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Trans_Phisic =
			modBusRtuSlaveDisableInterTransPhisic;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_Enable_Inter_Receiv_Phisic =
			modBusRtuSlaveEnableInterReceivPhisic;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_Enable_Inter_Trans_Phisic =
			modBusRtuSlaveEnableInterTransPhisic;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_RTS1_RX =
			modBusRtuSlaveRtsRx;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_RTS1_TX =
			modBusRtuSlaveRtsTx;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_Timer_Init =
			modBusRtuSlaveTimerInit;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_Timer_Start =
			modBusRtuSlaveTimerStart;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_Timer_Stop =
			modBusRtuSlaveTimerStop;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_UART_Init =
			modBusRtuSlaveUartInit;

	scan.mb.ModBusRTU_Slave.FunctionPeriphery.pModBusRTU_Slave_UART_Write_Phisic =
			modBusRtuSlaveUartWritePhisic;
}
