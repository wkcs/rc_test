C_SRCS += $(TOP_DIR)/usb/STM32_USB_Device_Library/Class/cdc/src/usbd_cdc_core.c
C_SRCS += $(wildcard $(TOP_DIR)/usb/STM32_USB_Device_Library/Core/src/*.c)
C_SRCS += $(TOP_DIR)/usb/STM32_USB_OTG_Driver/src/usb_core.c
C_SRCS += $(TOP_DIR)/usb/STM32_USB_OTG_Driver/src/usb_dcd.c
C_SRCS += $(TOP_DIR)/usb/STM32_USB_OTG_Driver/src/usb_dcd_int.c
C_SRCS += $(wildcard $(TOP_DIR)/usb/USB_APP/*.c)