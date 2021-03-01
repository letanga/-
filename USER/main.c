#include "stm32f10x.h"
#include "timer.h"
#include "motor.h"
#include "usart.h"
#include "led.h"
#include "I2C.h"
#include "systick.h"
#include "car.h"
#include "mpu6050.h"
#include "NRF24L01.h"

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�������ȼ�����2
	USART1_Config();//����1��ʼ�����������س���ʹ�ӡMPU6050��ص���־
	USART3_Config();
	TIM2_PWM_Init();//��ʱ��2��ʼ�����ڲ���PWM�����Ƶ��
	TIM3_Encoder_Init(); //��ʱ��3��ʼ����������ģʽ���ڼ��㳵��
	TIM4_Encoder_Init();//��ʱ��4��ʼ����������ģʽ���ڼ��㳵��
	//NRF24L01_init();
	MOTOR_GPIO_Config();//���������ʼ��
	LED_GPIO_Config();//LED��ʼ��
	i2cInit();//I2C��ʼ��
	delay_nms(10);
	MPU6050_Init();	//MPU6050��ʼ��
	CarUpstandInit();//ֱ��������ʼ��
	SysTick_Init();//Systick��ʼ������������ϵͳ����
	while(1)
	{
		MPU6050_Pose();//���ϻ�ȡ��̬
	}
}

