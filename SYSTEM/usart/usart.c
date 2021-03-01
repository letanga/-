#include "usart.h"
#include <stdarg.h>
#include "misc.h"
#include "stm32f10x_usart.h"
#include "Remote.h"
//���ڽ���DMA����
u8 newLineReceived = 0;
u8 inputString[80] = {0};
uint8_t Uart_Rx[UART_RX_LEN] = {0};
int num = 0;
u8 startBit = 0;
int int9num =0;
u8 Fore,Back,Left,Right;
u8 Bluetooth_data;
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//�����ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;//�����ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIO�ʹ���ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO PA9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO PA10
	
	USART_InitStructure.USART_BaudRate = 115200;//���ò�����115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ͽ���ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
}

void USART3_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	//���崮�ڳ�ʼ���ṹ��
	
	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
		/* config GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB	, ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull �������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART3 Rx (PB.11) as input floating ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	
	
	NVIC_Configurationusart3(); 
	/* USART3 mode config */
	//USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_BaudRate = 9600;//������9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����RTSCTSӲ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ�ܷ��ͽ���
  

	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	
	USART_Cmd(USART3, ENABLE);
	
}




void NVIC_Configurationusart3(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART3_IRQHandler(void) 
{
	
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�����жϱ�־λ����
	{
		Bluetooth_data=USART_ReceiveData(USART3);//������յ�����
		
		if(Bluetooth_data==0x00)Fore=0,Back=0,Left=0,Right=0;//ɲ
		else if(Bluetooth_data==0x01)Fore=1,Back=0,Left=0,Right=0;//ǰ
		else if(Bluetooth_data==0x05)Fore=0,Back=1,Left=0,Right=0;//��
		else if(Bluetooth_data==0x03)Fore=0,Back=0,Left=1,Right=0;//��
		else if(Bluetooth_data==0x07)Fore=0,Back=0,Left=0,Right=1;//��
		else										Fore=0,Back=0,Left=0,Right=0;//ɲ
	}
}
/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
	/* ��Printf���ݷ������� */
	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}



//����ַ���������1
void PrintChar(char *s)
{
	char *p;
	p=s;
	while(*p != '\0')
	{
		USART_SendData(USART1, *p);        //ͨ���⺯��  ��������
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);//�ȴ�������ɡ�   ��� USART_FLAG_TC �Ƿ���1��    //���⺯�� P359 ����
		p++;
	}
}

