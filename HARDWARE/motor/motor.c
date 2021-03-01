#include "motor.h"
#include "stm32f10x.h"

void MOTOR_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;//�����ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//ѡ��Ҫ���Ƶ�GPIOB����	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪͨ���������  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//������������Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO	  
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_12 | GPIO_Pin_15);//ȫ����ʼ��Ϊ�ߵ�ƽ���ƶ�״̬
}
