#include "led.h"
#include "stm32f10x.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//�����ʼ���ṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);//ʹ��GPIOB��AFIO������ʱ��
	
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ��													   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//ѡ��Ҫ���Ƶ�GPIOB����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//������������Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��������ģʽΪͨ���������
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOB	 
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);//��led��
}

