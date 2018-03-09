#工程的名称及最后生成文件的名字
TARGET = rc11x_test

#读取当前工作目录
TOP_DIR = .

include config.mk

LIB_MAKE_DIR += $(TOP_DIR)/cmsis/st_lib

LIBS += $(TOP_DIR)/cmsis/st_lib

# 宏定义
#DEFS		= -D ARM_MATH_CM4=1U -D STM32F407xx -D USE_HAL_DRIVER -D __MPU_PRESENT=1U
DEFS		= -D STM32F40_41xxx -D USE_STDPERIPH_DRIVER -D __VFP_FP__ -D USE_USB_OTG_FS 

#链接脚本
LDSCRIPT    = $(TOP_DIR)/stm32_flash.ld

# OpenOCD specific variables
OOCD		:= openocd
OOCD_INTERFACE	:= flossjtag
OOCD_TARGET	:= stm32f4x
OOCDFLAGS := -f openocd.cfg

#设定包含文件目录
INC_DIR = -I $(TOP_DIR)/board/v3          \
          -I $(TOP_DIR)/cmsis/core        \
		  -I $(TOP_DIR)/cmsis/sys         \
		  -I $(TOP_DIR)/cmsis/st_lib/inc  \
		  -I $(TOP_DIR)/com               \
		  -I $(TOP_DIR)/drivers           \
		  -I $(TOP_DIR)/hal/inc           \
		  -I $(TOP_DIR)/init              \
          -I $(TOP_DIR)/kernel            \
          -I $(TOP_DIR)/lib               \
          -I $(TOP_DIR)/rc_test           \
		  -I $(TOP_DIR)/cmd               \
		  -I $(TOP_DIR)/usb/STM32_USB_Device_Library/Class/cdc/inc \
		  -I $(TOP_DIR)/usb/STM32_USB_Device_Library/Core/inc \
		  -I $(TOP_DIR)/usb/STM32_USB_OTG_Driver/inc \
		  -I $(TOP_DIR)/usb/USB_APP

#SOURCE_DIRS = $(TOP_DIR)/core          \
              $(TOP_DIR)/cmd           \
			  $(TOP_DIR)/cmd/src       \
			  $(TOP_DIR)/lib           \
              $(TOP_DIR)/drivers       \
			  $(TOP_DIR)/hal           \
			  $(TOP_DIR)/hal/src       \
			  $(TOP_DIR)/system        \
			  $(TOP_DIR)/user          \
			  $(TOP_DIR)/kernel

#DEBUG_DIRS = $(TOP_DIR)/debug
#DEBUG_DIRS += $(SOURCE_DIRS:./%=$(TOP_DIR)/debug/%)


CCFLAGS += $(INC_DIR)
CCFLAGS += $(DEFS)



ASFLAGS += $(INC_DIR)
ASFLAGS += $(DEFS)


LDFLAGS += -T $(LDSCRIPT)
LDFLAGS += $(LIBS:%=-L %)

# 展开工作 子目录中的inc文件（inc文件中添加需要编译链接的.c，.s等文件）
-include $(TOP_DIR)/board/v3/make.inc
-include $(TOP_DIR)/cmsis/core/make.inc
-include $(TOP_DIR)/cmsis/sys/make.inc
-include $(TOP_DIR)/com/make.inc
-include $(TOP_DIR)/drivers/make.inc
#-include $(TOP_DIR)/hal/make.inc
-include $(TOP_DIR)/init/make.inc
-include $(TOP_DIR)/kernel/make.inc
-include $(TOP_DIR)/lib/make.inc
-include $(TOP_DIR)/rc_test/make.inc
-include $(TOP_DIR)/cmd/make.inc
-include $(TOP_DIR)/usb/make.inc

 
C_OBJS = $(C_SRCS:./%.c=$(TOP_DIR)/debug/%.o)
ASM_OBJS = $(ASM_SRCS:./%.S=$(TOP_DIR)/debug/%.o)
OBJS = $(C_OBJS) 
OBJS += $(ASM_OBJS)
DEPS = $(OBJS:%.o=%.d)

SECONDARY_FLASH = $(TARGET).hex
SECONDARY_SIZE = $(TARGET).siz
SECONDARY_BIN = $(TARGET).bin


.PHONY: images clean size bin hex size list debug stlink-flash style-code flash debug_file


all: $(TARGET).images
	@echo   building done

size: $(TARGET).elf
	@echo   SIZE    $(TARGET).elf
	$(Q)$(SIZE) --format=berkeley "$(TARGET).elf"

bin: $(TARGET).bin

hex: $(TARGET).hex

#style-code:
#	sh $(TOP_DIR)/buildtool/stylecode.sh

$(TARGET).images: $(TARGET).bin $(TARGET).hex $(TARGET).list
	$(Q)echo   images generated


$(TARGET).elf: $(OBJS) $(LDSCRIPT) libs
	@echo   LD      $(TARGET).elf
	$(Q)$(CC) $(LDFLAGS) -o "$(TARGET).elf" $(C_OBJS) $(ASM_OBJS)
	@echo   SIZE    $(TARGET).elf\n\n
	$(Q)$(SIZE) --format=berkeley "$(TARGET).elf"



$(TARGET).hex: $(TARGET).elf
	@echo   OBJCOPY $(TARGET).hex
	$(Q)$(OBJCOPY) $(TARGET).elf  $(TARGET).hex -Oihex


$(TARGET).bin: $(TARGET).elf
	@echo   OBJCOPY $(TARGET).bin
	$(Q)$(OBJCOPY) $(TARGET).elf  $(TARGET).bin -Obinary

$(TARGET).list: $(TARGET).elf
	@echo   OBJDUMP $(TARGET).list
	$(Q)$(OBJDUMP) -S $(TARGET).elf > $(TARGET).list


$(C_OBJS):$(TOP_DIR)/debug/%.o:$(TOP_DIR)/%.c 
	@echo   CC      $<
	$(Q)$(CC) $(CCFLAGS) -o $@ $<

$(ASM_OBJS):$(TOP_DIR)/debug/%.o:$(TOP_DIR)/%.S 
	@echo   AS      $<
	$(Q)$(CC) $(ASFLAGS) -o $@ $<

libs:
	$(MAKE) -C $(LIB_MAKE_DIR) $@
#debug_file:
#	mkdir $(DEBUG_DIRS)

# 使用stlink驱动下载bin程序
stlink-flash: $(TARGET).bin
	@echo   ST-link FLASH  $<
	$(Q)$(STFLASH) write $(TARGET).bin 0x8000000

# 使用OpenOCD下载hex程序
flash: $(TARGET).hex
	@echo   OPEN_OCD FLASH $<
	$(Q)$(OOCD) $(OOCDFLAGS) -c "program $(TARGET).hex verify reset exit" 

# 使用GDB 通过sdtin/out管道与OpenOCD连接 并在main函数处打断点后运行
debug: $(TARGET).elf
	@echo   GDB DEBUG $<
	$(Q)$(GDB) -iex 'target extended | $(OOCD) $(OOCDFLAGS) -c "gdb_port pipe"' \
	-iex 'monitor reset halt' -ex 'load' -ex 'break rc_test_loop' -ex 'c' $(TARGET).elf

-include $(DEPS)

clean:
	@echo   CLEAN
	$(Q)-$(RM) $(DEPS) $(OBJS) $(TARGET).elf $(TARGET).map $(TARGET).list $(TARGET).bin $(TARGET).hex
	$(MAKE) -C $(TOP_DIR)/cmsis/st_lib $@