##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.6.0] date: [Thu Oct 08 18:52:11 CEST 2020]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = FLogger


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_F103SOURCES = \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_sdmmc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sd.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c \
Src/F103/system_stm32f1xx.c \
Src/F103/stm32f1xx_it.c \
Src/F103/stm32f1xx_hal_msp.c \
Src/F103/usb_device.c \
Src/F103/usbd_conf.c \
Src/F103/usbd_desc.c \
Src/F103/usbd_cdc_if.c \
Src/F103/stm32f1xx_hal_mxinit.c

C_F205SOURCES = \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pcd_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pcd.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_usb.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cortex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pwr.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pwr_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_gpio.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_exti.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_adc.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_adc_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_crc.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dac.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dac_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_i2c.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rtc.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rtc_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_spi.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim_ex.c \
Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_uart.c \
Src/F205/system_stm32f2xx.c \
Src/F205/usb_device.c \
Src/F205/usbd_conf.c \
Src/F205/usbd_desc.c \
Src/F205/usbd_cdc_if.c \
Src/F205/stm32f2xx_it.c \
Src/F205/stm32f2xx_hal_msp.c \
Src/F205/stm32f2xx_hal_mxinit.c

C_SOURCES =  \
Src/printf.c \
Src/cppabi.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c

CPP_SOURCES = \
Src/main.cpp \
Src/FwLogger.cpp \
Src/SDI12_Driver.cpp \
Src/SPI_Device.cpp \
Src/SPIFlash.cpp \
Src/SFS.cpp \
Src/LoRa.cpp \
Src/Module.cpp \
Src/VirtualMachine.cpp \
Src/VMBuiltin.cpp \
Src/io.cpp \
Src/Log.cpp \
Src/LinkLayer.cpp \
Src/SSD1306.cpp \
Src/Digital_Driver.cpp \
Src/Connectivity.cpp

# ASM sources
ASM_F103SOURCES =  \
startup_stm32f103xe.s

ASM_F205SOURCES = \
startup_stm32f205xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CPP = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CPP = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F103xE \
-DCPU_STRING_MODEL=\"STM32F103RET6\"


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/CMSIS/Include \
-IMiddlewares/ST/STM32_USB_Device_Library/Core/Inc \
-IMiddlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CPPFLAGS = -fno-rtti -fno-exceptions -std=c++11 -fno-threadsafe-statics

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103RETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

ifdef F205
C_SOURCES += $(C_F205SOURCES)
ASM_SOURCES = $(ASM_F205SOURCES)
LDSCRIPT = STM32F205RETx_FLASH.ld

C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F205xx \
-DCPU_STRING_MODEL=\"STM32F205RET6\"

C_INCLUDES += \
-IInc/F205 \
-IDrivers/STM32F2xx_HAL_Driver/Inc \
-IDrivers/STM32F2xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F2xx/Include

else
C_SOURCES += $(C_F103SOURCES)
ASM_SOURCES = $(ASM_F103SOURCES)

C_INCLUDES += \
-IInc/F103 \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include

endif

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CPP) -c $(CFLAGS) $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

cleanall: clean

ocd:
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg

flash: all
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
