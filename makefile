﻿PROJECT_NAME		:= scanEl

DEFINE_PROJ			:=	-DSTM32F415xx
DEFINE_PROJ			+=	-DSTM32F4
DEFINE_PROJ			+=	-DSTM32
#DEFINE_PROJ			+=	-DDEBUG
DEFINE_PROJ			+=	-D__VFP_FP__
DEFINE_PROJ			+=	-D__FPU_PRESENT

DEFINE_PROJ			+=	-DMODULE_ARITHMETIC_MEAN_ENABLED
DEFINE_PROJ			+=	-DMODULE_CPP_SYSTEM_CALLS_DUMMY_ENABLED
DEFINE_PROJ			+=	-DHAL_ADC_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_DAC_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_DMA_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_GPIO_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_PWR_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_RCC_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_TIM_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_UART_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_WWDG_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_FLASH_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_CORTEX_MODULE_ENABLED
DEFINE_PROJ			+=	-DHAL_MODULE_ENABLED

CODE_OPTIMIZATION	:=	-Ofast -g0

LD_FILES			=	-T submodule/module_stm32f4_low_level_by_st/ld/STM32F205xG.ld
STARTUPE_S_NAME		=	submodule/module_stm32f4_low_level_by_st/startupe/startup_stm32f205xx.s

MK_FLAGS			:=	-mcpu=cortex-m4
MK_FLAGS			+=	-mthumb
MK_FLAGS			+=	-mfloat-abi=hard
MK_FLAGS			+=	-mfpu=fpv4-sp-d16
MK_FLAGS			+=	--specs=nano.specs	

C_FLAGS				:= $(MK_FLAGS) 
# Все предупреждения == ошибки.
#C_FLAGS			+= -Werror
# Выдавать предупреждения (ошибки) о сомнительных констукциях.
#C_FLAGS			+= -Wall
# Выдавать предупреждение (ошибку) о любых сомнительных действиях.
#C_FLAGS			+= -Wextra 
C_FLAGS				+= -std=c99 
# Если переменная объявлена как enum, то она должна иметь возможность
# хранить в себе всевозможные состояния этого enum-а (а не только текущее).
C_FLAGS				+= -fshort-enums

CPP_FLAGS			:=	$(MK_FLAGS)
CPP_FLAGS			+=	-Werror
CPP_FLAGS			+=	-Wall
CPP_FLAGS			+=	-Wextra
CPP_FLAGS			+=	-std=c++14
CPP_FLAGS			+=	-fno-exceptions
CPP_FLAGS			+=	-fshort-enums	

# Linker.
LD_FILES			:=	-T submodule/module_stm32f4_low_level_by_st/LD/STM32F415RGTx_FLASH.ld
STARTUPE_S_NAME		:=	submodule/module_stm32f4_low_level_by_st/startupe/startup_stm32f415xx.s

#Linker flags.
LDFLAGS				:=	$(MK_FLAGS) $(LD_FILES) -fno-exceptions
LDFLAGS				+=	-Wl,--undefined=uxTopUsedPriority			# FreeRTOS.
LDFLAGS				+=	-ffunction-sections -Wl,--gc-sections

# Формируем map файл.
LDFLAGS			+= -Wl,-Map="build/$(PROJECT_NAME).map"

#**********************************************************************
# Параметры toolchain-а.
#**********************************************************************
TOOLCHAIN_PATH	= arm-none-eabi

CC				= $(TOOLCHAIN_PATH)-gcc
CPP				= $(TOOLCHAIN_PATH)-g++
CCDEP			= $(TOOLCHAIN_PATH)-gcc
LD				= $(TOOLCHAIN_PATH)-g++
AR				= $(TOOLCHAIN_PATH)-ar
AS				= $(TOOLCHAIN_PATH)-gcc
OBJCOPY			= $(TOOLCHAIN_PATH)-objcopy
OBJDUMP			= $(TOOLCHAIN_PATH)-objdump
GDB				= $(TOOLCHAIN_PATH)-gdb
SIZE			= $(TOOLCHAIN_PATH)-size

# Все файлы из папки cfg в каталоге проекта.
PROJ_CFG_H_FILE		:= $(wildcard cfg/*.h)
PROJ_CFG_DIR		:= cfg
PROJ_CFG_PATH		:= -I$(PROJ_CFG_DIR)

PROJECT_PATH		+= $(PROJ_CFG_PATH)

PROJ_H_FILE				:= $(shell find user_code/ -maxdepth 10 -type f -name "*.h" )
PROJ_H_FILE				+= $(shell find submodule/ -maxdepth 10 -type f -name "*.h" )

PROJ_CPP_FILE			:= $(shell find user_code/ -maxdepth 10 -type f -name "*.cpp" )
PROJ_CPP_FILE			+= $(shell find submodule/ -maxdepth 10 -type f -name "*.cpp" )

PROJ_C_FILE				:= $(shell find user_code/ -maxdepth 10 -type f -name "*.c" )
PROJ_C_FILE				+= $(shell find submodule/ -maxdepth 10 -type f -name "*.c" )

PROJ_S_FILE				= $(STARTUPE_S_NAME)

PROJ_DIR				:= $(shell find user_code/ -maxdepth 10 -type d -name "*" )
PROJ_DIR				+= $(shell find submodule/ -maxdepth 10 -type d -name "*" )

PROJ_PATH				:= $(addprefix -I, $(PROJ_DIR))
PROJ_OBJ_FILE			:= $(addprefix build/obj/, $(PROJ_CPP_FILE))
PROJ_OBJ_FILE			+= $(addprefix build/obj/, $(PROJ_C_FILE))
PROJ_OBJ_FILE			+= $(addprefix build/obj/, $(PROJ_S_FILE))

PROJ_OBJ_FILE			:= $(patsubst %.cpp, %.o, $(PROJ_OBJ_FILE))
PROJ_OBJ_FILE			:= $(patsubst %.c, %.o, $(PROJ_OBJ_FILE))
PROJ_OBJ_FILE			:= $(patsubst %.s, %.o, $(PROJ_OBJ_FILE))

PROJECT_PATH			+= $(PROJ_PATH)
PROJECT_OBJ_FILE		+= $(PROJ_OBJ_FILE)

build/obj/%.o:	%.s
	@echo [AS] $<
	@mkdir -p $(dir $@)
	@$(AS)								\
	$(DEFINE_PROJ)						\
	$(CODE_OPTIMIZATION)				\
	$(PROJECT_PATH)						\
	-c $< -o $@


build/obj/%.o:	%.c	
	@echo [CC] $<
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) 					\
	$(DEFINE_PROJ)						\
	$(CODE_OPTIMIZATION)				\
	$(PROJECT_PATH)						\
	-c $< -o $@
	
build/obj/%.o:	%.cpp	
	@echo [CPP] $<
	@mkdir -p $(dir $@)
	@$(CPP) $(CPP_FLAGS) 				\
	$(DEFINE_PROJ)						\
	$(CODE_OPTIMIZATION)				\
	$(PROJECT_PATH)						\
	-c $< -o $@

build/$(PROJECT_NAME).elf:	$(PROJECT_OBJ_FILE)
	@$(LD) $(LDFLAGS) $(PROJECT_OBJ_FILE)  -o build/$(PROJECT_NAME).elf
	@echo ' '
	@echo 'Finished building target: $@'
	@echo ' '

$(PROJECT_NAME).siz:	build/$(PROJECT_NAME).elf
	@echo 'Print Size:'
	@arm-none-eabi-size --format=berkeley "build/$(PROJECT_NAME).elf"
	@echo ' '

all:	$(PROJECT_NAME).siz
#@$(OBJDUMP) -D build/$(PROJECT_NAME).elf > build/$(PROJECT_NAME).asm
#@$(OBJCOPY) build/$(PROJECT_NAME).elf build/$(PROJECT_NAME).bin -O binary

clean:	
	@rm -R ./build
	@echo 'Project cline!'
	@echo ' '
