#include "systick.h"
#include "stm32f10x.h"
#include "car.h"
#include "Remote.h"

u16 TimeDelay=0;

extern void SysTick_Init(void)		  //1ms��ʱʱ��
{
	if (SysTick_Config(SystemCoreClock / 200))	// ST3.5.0��汾   1s/x= a ms  ��5ms����һ��systick�ж�
 	{ 
		while(1);
	}
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)				 //5ms��ʱ��
{  
	BST_u8SpeedControlCount++;
	GetMotorPulse();						//������㺯��
	
	AngleControl();		//�Ƕ�PD����PWNM���
	Remote_Control();
	TurnControl();
	MotorOutput();					  //С����PWM���  	

  if(BST_u8SpeedControlCount>=8)       //������ֵ8ʱ������ϵͳ����40msʱ��(ÿ10���Ƕ�PWM���������1���ٶ�PWM����������ܱ����ٶ�PID��������ŽǶ�PID������Ӷ�Ӱ��С��ƽ��)
	{	
		
		SpeedControl();                     //��ģ�ٶȿ��ƺ���   ÿ40ms����һ��
		BST_u8SpeedControlCount=0;		  //С���ٶȿ��Ƶ��ü���ֵ����
		//BST_u8SpeedControlPeriod=0;		  //ƽ���������ֵ����
	}
}
