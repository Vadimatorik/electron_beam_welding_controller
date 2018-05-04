#pragma once

#include "modbus_rtu_slave.h"
#include "modbus_slave_registers.h"
#include "modbus_slave_function.h"
#include "modbus_rtu_portable.h"

struct scanModbus {
	// Структура данных ModBusRTU_Slave стека
	struct modbus_rtu_slave ModBusRTU_Slave;

	// Таблицы регистров
	struct modbus_slave_registers_table ModBusRTU_Slave_RegistersTable[2];

	// Уникальная карта полей таблиц регистров
	struct modbus_slave_registers_map_table ModBusRTU_Slave_RegistersMapTable;

	// ACCESS_REG_RO
	// RegMap_Table_1
	uint16_t RegMap_Table_1[5];

	// ACCESS_REG_RW
	// RegMap_Table_2
	uint16_t RegMap_Table_2[10];

	// Приемопередающий буфер
	uint8_t ModBusRTU_Slave_RxTxBuff[257];
};

void			modbusInit			( void );
