#include "usbd_usr.h" 
#include "usb_dcd_int.h"
#include "usart.h" 	   

//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;		//默认没有连接  


extern USB_OTG_CORE_HANDLE  USB_OTG_dev;

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{
  	USBD_OTG_ISR_Handler(&USB_OTG_dev);
}  
//指向DEVICE_PROP结构体
//USB Device 用户回调函数. 
USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,    
};
//USB Device 用户自定义初始化函数
void USBD_USR_Init(void)
{
	//printf("USBD_USR_Init\r\n");
} 
//USB Device 复位
//speed:USB速度,0,高速;1,全速;其他,错误.
void USBD_USR_DeviceReset (uint8_t speed)
{
	switch (speed)
	{
		case USB_OTG_SPEED_HIGH:
			//printf("USB Device Library v1.1.0  [HS]\r\n");
			break; 
		case USB_OTG_SPEED_FULL: 
			//printf("USB Device Library v1.1.0  [FS]\r\n");
			break;
		default:
			//printf("USB Device Library v1.1.0  [??]\r\n"); 
			break;
	}
}
//USB Device 配置成功
void USBD_USR_DeviceConfigured (void)
{
	//printf("MSC Interface started.\r\n"); 
} 
//USB Device挂起
void USBD_USR_DeviceSuspended(void)
{
	//printf("Device In suspend mode.\r\n");
} 
//USB Device恢复
void USBD_USR_DeviceResumed(void)
{ 
	//printf("Device Resumed\r\n");
}
//USB Device连接成功
void USBD_USR_DeviceConnected (void)
{
	bDeviceState=1;
	//printf("USB Device Connected.\r\n");
}
//USB Device未连接
void USBD_USR_DeviceDisconnected (void)
{
	bDeviceState=0;
	//printf("USB Device Disconnected.\r\n");
} 

void USB_Port_Set(uint8_t enable)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	

	if(enable)
		*(__IO uint32_t *)0x50000804 &= 0xfffffffd;
	else {
		*(__IO uint32_t *)0x50000804 |= 0x00000002;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
		PAout(11)=0; 
		PAout(12)=0;  
	}
}  
















