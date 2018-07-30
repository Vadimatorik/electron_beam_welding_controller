#pragma once

#include "modbus_rtu_slave.h"
#include "modbus_slave_registers.h"
#include "modbus_slave_function.h"
#include "modbus_rtu_portable.h"

struct scanModbus {
	// Структура данных ModBusRTU_Slave стека
	struct modbus_rtu_slave ModBusRTU_Slave;

	// Таблицы регистров
	struct modbus_slave_registers_table ModBusRTU_Slave_RegistersTable[1];

	// Уникальная карта полей таблиц регистров
	struct modbus_slave_registers_map_table ModBusRTU_Slave_RegistersMapTable;

	uint16_t RegMap_Table_1[ 1 + 512 + 1 + 1 ];

	// Приемопередающий буфер
	uint8_t ModBusRTU_Slave_RxTxBuff[257];
};

void			modbusInit			( void );
