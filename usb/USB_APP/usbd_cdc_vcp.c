#include "usbd_cdc_vcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>	  


//USB���⴮��������ò���
LINE_CODING linecoding =
{
	1382400,		//������
	0x00,   	//ֹͣλ,Ĭ��1λ
	0x00,   	//У��λ,Ĭ����
	0x08    	//����λ,Ĭ��8λ
}; 

u8  USART_PRINTF_Buffer[USB_USART_REC_LEN];	//usb_printf���ͻ�����

//�����ƴ���1�������ݵķ���,������USB���⴮�ڽ��յ�������.
u8 USB_USART_RX_BUF[USB_USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USB_USART_RX_STA=0;       				//����״̬���	 

uart_data_t uart_usb_data;

 
extern uint8_t  APP_Rx_Buffer [];			//���⴮�ڷ��ͻ�����(��������) 
extern uint32_t APP_Rx_ptr_in;   			//���⴮�ڽ��ջ�����(�������Ե��Ե�����)

//���⴮�����ú���(��USB�ں˵���)
CDC_IF_Prop_TypeDef VCP_fops = 
{
	VCP_Init,
	VCP_DeInit,
	VCP_Ctrl,
	VCP_DataTx,
	VCP_DataRx
}; 

//��ʼ��VCP
//����ֵ:USBD_OK
uint16_t VCP_Init(void)
{ 
	return USBD_OK;
} 
//��λVCP
//����ֵ:USBD_OK
uint16_t VCP_DeInit(void)
{ 
	return USBD_OK;
} 
//����VCP������
//buf:�������ݻ�����/�������滺����
//len:���ݳ���
//����ֵ:USBD_OK
uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
	switch (Cmd)
	{
		case SEND_ENCAPSULATED_COMMAND:break;   
		case GET_ENCAPSULATED_RESPONSE:break;  
		case SET_COMM_FEATURE:break;  
		case GET_COMM_FEATURE:break;  
 		case CLEAR_COMM_FEATURE:break;  
		case SET_LINE_CODING:
			linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
			linecoding.format = Buf[4];
			linecoding.paritytype = Buf[5];
			linecoding.datatype = Buf[6]; 
			//��ӡ���ò���
			//printf("linecoding.format:%d\r\n",linecoding.format);
			//printf("linecoding.paritytype:%d\r\n",linecoding.paritytype);
			//printf("linecoding.datatype:%d\r\n",linecoding.datatype);
			//printf("linecoding.bitrate:%d\r\n",linecoding.bitrate);
			break; 
		case GET_LINE_CODING:
			Buf[0] = (uint8_t)(linecoding.bitrate);
			Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
			Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
			Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
			Buf[4] = linecoding.format;
			Buf[5] = linecoding.paritytype;
			Buf[6] = linecoding.datatype; 
			break; 
		case SET_CONTROL_LINE_STATE:break;   
		case SEND_BREAK:break;   
		default:break;  
	} 
	return USBD_OK;
}
//����һ���ֽڸ����⴮��(��������)
//data:Ҫ���͵�����
//����ֵ:USBD_OK
uint16_t VCP_DataTx (uint8_t data)
{  
	APP_Rx_Buffer[APP_Rx_ptr_in]=data;	//д�뷢��buf
	APP_Rx_ptr_in++;  					//дλ�ü�1
	if(APP_Rx_ptr_in==APP_RX_DATA_SIZE)	//����buf��С��,����.
	{
		APP_Rx_ptr_in = 0;
	}   
	return USBD_OK;
} 
//�����USB���⴮�ڽ��յ�������
//databuffer:���ݻ�����
//Nb_bytes:���յ����ֽ���.
//����ֵ:USBD_OK
uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
	u8 i;
	u8 Res;
	for(i=0;i<Len;i++)
	{  
		Res = Buf[i]; 
		if (uart_usb_data.rx_status == 0)
		{
			if (uart_usb_data.rx_len < 2) //����Ƿ�������ͷ
			{
				uart_usb_data.rx_buf[uart_usb_data.rx_len] = Res;
				uart_usb_data.rx_len++;
				if ((uart_usb_data.rx_len == 2) && (uart_usb_data.rx_buf[1] == 0))
				{ //���ݰ�����Ϊ0ʱ�ж��Ƿ�������
					uart_usb_data.rx_status = 1;
				}
			}
			else if (uart_usb_data.rx_len < (uint32_t)(uart_usb_data.rx_buf[1] + 2)) //�ж��Ƿ������һ֡����
			{
				uart_usb_data.rx_buf[uart_usb_data.rx_len] = Res;
				uart_usb_data.rx_len++;
				if (uart_usb_data.rx_len > 256)
					uart_usb_data.rx_status = 0; //�������ݴ���,���¿�ʼ����
				if (uart_usb_data.rx_len == (uint32_t)(uart_usb_data.rx_buf[1] + 2))
				{
					uart_usb_data.rx_status = 1;
				}
			}
		}   
	}
	return USBD_OK;
}
//usb���⴮��,printf ����
//ȷ��һ�η������ݲ���USB_USART_REC_LEN�ֽ�
void usb_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART_PRINTF_Buffer,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART_PRINTF_Buffer);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		VCP_DataTx(USART_PRINTF_Buffer[j]); 
	}
} 



























