#include "usbd_usr.h" 
#include "usb_dcd_int.h"
#include "usart.h" 	   

//��ʾUSB����״̬
//0,û������;
//1,�Ѿ�����;
vu8 bDeviceState=0;		//Ĭ��û������  


extern USB_OTG_CORE_HANDLE  USB_OTG_dev;

//USB OTG �жϷ�����
//��������USB�ж�
void OTG_FS_IRQHandler(void)
{
  	USBD_OTG_ISR_Handler(&USB_OTG_dev);
}  
//ָ��DEVICE_PROP�ṹ��
//USB Device �û��ص�����. 
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
//USB Device �û��Զ����ʼ������
void USBD_USR_Init(void)
{
	//printf("USBD_USR_Init\r\n");
} 
//USB Device ��λ
//speed:USB�ٶ�,0,����;1,ȫ��;����,����.
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
//USB Device ���óɹ�
void USBD_USR_DeviceConfigured (void)
{
	//printf("MSC Interface started.\r\n"); 
} 
//USB Device����
void USBD_USR_DeviceSuspended(void)
{
	//printf("Device In suspend mode.\r\n");
} 
//USB Device�ָ�
void USBD_USR_DeviceResumed(void)
{ 
	//printf("Device Resumed\r\n");
}
//USB Device���ӳɹ�
void USBD_USR_DeviceConnected (void)
{
	bDeviceState=1;
	//printf("USB Device Connected.\r\n");
}
//USB Deviceδ����
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
		PAout(11)=0; 
		PAout(12)=0;  
	}
}  
















