#读取当前工作目录
TOP_DIR = .

include $(TOP_DIR)/../../config.mk

DEFS		= -D STM32F40_41xxx -D USE_STDPERIPH_DRIVER -D __VFP_FP__

INC_DIR = -I $(TOP_DIR)/inc  
INC_DIR += -I $(TOP_DIR)/../sys
INC_DIR += -I $(TOP_DIR)/../core

CCFLAGS += $(INC_DIR)
CCFLAGS += $(DEFS)

ARFLAGS = cr 

include $(TOP_DIR)/make.inc

C_OBJS = $(STM32_LIB_SRCS:./%.c=./../../debug/cmsis/st_lib/%.o)
OBJS = $(C_OBJS)
DEPS = $(OBJS:%.o=%.d)

all: libs

libs: stm32_lib.a

stm32_lib.a: $(C_OBJS)
	@echo   AR stm32_lib.a
	$(Q)$(AR) $(ARFLAGS) -o $@ $(OBJS)

$(C_OBJS): $(TOP_DIR)/../../debug/cmsis/st_lib/%.o:$(TOP_DIR)/%.c 
	@echo   CC      $<
	$(Q)$(CC) $(CCFLAGS) -o $@ $<

.PHONY: libs clean all

clean:
	@echo   CLEAN
	$(Q)-$(RM) $(OBJS) 
	$(Q)-$(RM) $(DEPS) 
	$(Q)-$(RM) stm32_lib.a