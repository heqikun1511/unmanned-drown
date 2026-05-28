TARGET = atk_f103
BUILD_DIR = build
OUTPUT_DIR = Output

PREFIX ?= arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size

CPU = -mcpu=cortex-m3 -mthumb
FPU =
FLOAT_ABI =
MCU = $(CPU) $(FPU) $(FLOAT_ABI)

DEFS = -DUSE_HAL_DRIVER -DSTM32F103xE

INCLUDE_DIRS = \
  App \
  Drivers \
  Drivers/CMSIS/Include \
  Drivers/CMSIS/Device/ST/STM32F1xx/Include \
  Drivers/STM32F1xx_HAL_Driver/Inc \
  Drivers/STM32F1xx_HAL_Driver/Inc/Legacy \
  Middlewares \
  Middlewares/FreeRTOS/include \
  Middlewares/FreeRTOS/portable/GCC/ARM_CM3 \
  Hardware \
  CubeMX/f01_flight/Core/Inc

INCLUDES = $(addprefix -I,$(INCLUDE_DIRS))

C_SOURCES = \
  App/main.c \
  Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c \
  App/stm32f1xx_it.c \
  App/freertos_demo.c \
  App/syscalls.c \
  Drivers/SYSTEM/delay/delay.c \
  Drivers/SYSTEM/sys/sys.c \
  Drivers/SYSTEM/usart/usart.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_fsmc.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
  Drivers/BSP/LED/led.c \
  Drivers/BSP/LCD/lcd.c \
  Drivers/BSP/KEY/key.c \
  Drivers/BSP/SRAM/sram.c \
  Drivers/BSP/TIMER/btim.c \
  Middlewares/MALLOC/malloc.c \
  Middlewares/FreeRTOS/croutine.c \
  Middlewares/FreeRTOS/event_groups.c \
  Middlewares/FreeRTOS/list.c \
  Middlewares/FreeRTOS/queue.c \
  Middlewares/FreeRTOS/stream_buffer.c \
  Middlewares/FreeRTOS/tasks.c \
  Middlewares/FreeRTOS/timers.c \
  Middlewares/FreeRTOS/portable/MemMang/heap_4.c \
  Middlewares/FreeRTOS/portable/GCC/ARM_CM3/port.c \
  Hardware/int_IP5305T.c \
  Hardware/int_led.c \
  Hardware/int_motor.c \
  Hardware/mpu6050.c \
  Hardware/SI24R1.c \
  CubeMX/f01_flight/Core/Src/gpio.c \
  CubeMX/f01_flight/Core/Src/spi.c \
  CubeMX/f01_flight/Core/Src/i2c.c \
  CubeMX/f01_flight/Core/Src/tim.c \
  CubeMX/f01_flight/Core/Src/stm32f1xx_hal_msp.c

ASM_SOURCES = Linker/startup_stm32f103xe_gcc.s

OBJECTS = $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.s=.o))

DEPENDS = $(OBJECTS:.o=.d)

CFLAGS = $(MCU) $(DEFS) $(INCLUDES) -O0 -g3 -Wall -ffunction-sections -fdata-sections -MMD -MP
ASFLAGS = $(MCU) -x assembler-with-cpp -MMD -MP
LDSCRIPT = Linker/STM32F103ZE_FLASH.ld
LDFLAGS = $(MCU) -T$(LDSCRIPT) -Wl,-Map=$(OUTPUT_DIR)/$(TARGET).map,--cref -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs -u _printf_float
LDLIBS = -lc -lm -lnosys

.PHONY: all clean flash size

all: $(OUTPUT_DIR)/$(TARGET).elf $(OUTPUT_DIR)/$(TARGET).hex $(OUTPUT_DIR)/$(TARGET).bin size

$(OUTPUT_DIR)/$(TARGET).elf: $(OBJECTS) $(LDSCRIPT) | $(OUTPUT_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

$(OUTPUT_DIR)/%.hex: $(OUTPUT_DIR)/%.elf | $(OUTPUT_DIR)
	$(OBJCOPY) -O ihex $< $@

$(OUTPUT_DIR)/%.bin: $(OUTPUT_DIR)/%.elf | $(OUTPUT_DIR)
	$(OBJCOPY) -O binary -S $< $@

$(BUILD_DIR)/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile
	@mkdir -p $(dir $@)
	$(AS) -c $(ASFLAGS) $< -o $@

$(OUTPUT_DIR):
	mkdir -p $@

size: $(OUTPUT_DIR)/$(TARGET).elf
	$(SIZE) $<

clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_DIR)/$(TARGET).elf $(OUTPUT_DIR)/$(TARGET).hex $(OUTPUT_DIR)/$(TARGET).bin $(OUTPUT_DIR)/$(TARGET).map

flash: $(OUTPUT_DIR)/$(TARGET).hex
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $< verify reset exit"

-include $(DEPENDS)
