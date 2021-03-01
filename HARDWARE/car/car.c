#include "car.h"
#include "stm32f10x.h"
#include "mpu6050.h"
#include "timer.h"
#include "Remote.h"

float CAR_SPEED_SET=0;
float CAR_ZERO_ANGLE=0;

float BST_fCarSpeed;//���ӵ�ǰ�ٶ�ֵ
float BST_fCarSpeedOld;//������һ���ٶ�ֵ
float BST_TurnSpeed=0;

float BST_fCarPosition;//����λ��
float BST_fCarAngle;//���ӽǶ�
float BST_fAngleControlOut;//�Ƕȿ���PWMֵ
float BST_fSpeedControlOut;//�ٶȿ���PWMֵ
float BST_fLeftMotorOut;//�������PWMֵ
float BST_fRightMotorOut;//�ҵ�����PWMֵ

float BST_u8MainEventCount=0;//����5ms��ʱ���ӳ���SysTick_Handler(void)�����жϼ���λ
float BST_u8SpeedControlCount=0;//����5ms��ʱ���ӳ���SysTick_Handler(void)��50ms�ٶ�ƽ���������λ
float BST_u8SpeedControlPeriod=0;//����5ms��ʱ���ӳ���SysTick_Handler(void)��50ms�ٶ�ƽ���������λ


u8 BST_u8DirectionControlPeriod;
u8 BST_u8DirectionControlCount;//ת�����ѭ������  ��SysTick_Handler(void)��ʹ�� ÿ5ms��1 


/******������Ʋ���******/
float BST_fSpeedControlOut;//�ٶȿ���PWM
float BST_fSpeedControlOutOld;
float BST_fSpeedControlOutNew;
float BST_fAngleControlOut;
float BST_fTurnControlOut;
float BST_fLeftMotorOut;
float BST_fRightMotorOut;

float BST_fCarAngle;//�Ƕȿ���PWM
float gyro_z;
float gyrx;
float gy0;

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*///���²ο�Ϊ�ص���Բο���ͬ��ص�ѹ�йأ������õ��ٵ���
float  BST_fCarAngle_P =91.3;//	91.3 //����Сʱ�����Ұڣ�����ʱ����  ����������ܹ�վ�� P=91.3�����ڸ�С�����˶�����ʹ��
float  BST_fCarAngle_D =0.22;	// 0.001 0.002 0.004 0.008 0.0010 0.011	 ��Сʱ��Ӧ��������ʱ�����

float  BST_fCarSpeed_P=5.1;
float  BST_fCarSpeed_I=0.10;

float BST_fCarTurn_Kd=0.24;
float BST_fCarTurn_Kp=0;

const double PID_Original[4] ={91.3, 0.21, 5.1, 0.10}; 
char  alldata[80];
char *iap;

/******�ٶȿ��Ʋ���******/
s16   BST_s16LeftMotorPulse;//����������
s16	  BST_s16RightMotorPulse;//�ҵ��������

s32   BST_s32LeftMotorPulseOld;
s32   BST_s32RightMotorPulseOld;
s32   BST_s32LeftMotorPulseSigma;//50ms��������ֵ
s32   BST_s32RightMotorPulseSigma;//50ms�ҵ������ֵ

float BST_fCarSpeed;//�������̵ó��ĳ���
float BST_fCarSpeedOld;

float BST_fCarPosition;//��������ͨ������õ���С��λ��

/*-----��ͣ����-----*/
int leftstop=0;
int rightstop=0;
int stopflag=0;

void CarUpstandInit(void)
{
	BST_s16LeftMotorPulse = BST_s16RightMotorPulse = 0;//��������ֵ��ʼ��
	BST_s32LeftMotorPulseSigma = BST_s32RightMotorPulseSigma = 0;//������������ʼ��

	BST_fCarSpeed = BST_fCarSpeedOld = 0;//ƽ��С������	��ʼ��
	BST_fCarPosition = 0;//ƽ��С��λ����	��ʼ��
	BST_fCarAngle= 0;//ƽ��С������	��ʼ��

	BST_fAngleControlOut = BST_fSpeedControlOut=0;//�Ƕ�PWM������PWM����������PWM��ʼ��
	BST_fLeftMotorOut= BST_fRightMotorOut   = 0;//���ҳ���PWM���ֵ��ʼ��
	
  BST_u8MainEventCount=0;//����5ms��ʱ���ӳ���SysTick_Handler(void)�����жϼ���λ
	BST_u8SpeedControlCount=0;//����5ms��ʱ���ӳ���SysTick_Handler(void)��50ms�ٶ�ƽ���������λ
  BST_u8SpeedControlPeriod=0;//����5ms��ʱ���ӳ���SysTick_Handler(void)��50ms�ٶ�ƽ���������λ
}

void GetMotorPulse(void)//�ɼ�����ٶ�����
{ 
	uint16_t u16TempLeft;
	uint16_t u16TempRight;
	
	u16TempLeft = TIM_GetCounter(TIM3);//TIM3��ʱ���������
 	u16TempRight= TIM_GetCounter(TIM4);	//TIM4��ʱ���������
	leftstop=u16TempLeft;
	rightstop=u16TempRight;
	TIM_SetCounter(TIM3,0);//TIM3->CNT = 0;
	TIM_SetCounter(TIM4,0);//TIM4->CNT = 0;//����
	BST_s16LeftMotorPulse=u16TempLeft;
	BST_s16RightMotorPulse=(-u16TempRight);
		
	BST_s32LeftMotorPulseSigma  +=BST_s16LeftMotorPulse;//����ֵ���� 40ms����ֵ
	BST_s32RightMotorPulseSigma +=BST_s16RightMotorPulse;//����ֵ���� 40ms����ֵ
}

void AngleControl(void)	 
{
	if(Pitch==0||Pitch<-20||Pitch>20)//MPU6050״ָ̬ʾ�� STM32���İ� PC13 ��ɫ������Ϊ������
	{
		  //Pitch=1;
	  GPIO_ResetBits(GPIOC, GPIO_Pin_13);//MPU6050״ָ̬ʾ�� STM32���İ� PC13 ��ɫ������Ϊ������
	}
	else 
	{GPIO_SetBits(GPIOC, GPIO_Pin_13);}//MPU6050״̬����ʱLED��Ϩ��
	
	BST_fCarAngle = Roll- CAR_ZERO_ANGLE;//DMP ROLL��������Ƕ���Ԥ��С����б�Ƕ�ֵ�Ĳ�ó��Ƕ�   
	BST_fAngleControlOut =  BST_fCarAngle * BST_fCarAngle_P + gyro[0] * BST_fCarAngle_D ;	  //�Ƕ�PD����							   
}

void TurnControl(void)
{
	BST_fTurnControlOut=BST_fCarTurn_Kd*gyro[2]+BST_fCarTurn_Kp*BST_TurnSpeed;
}

void SpeedControl(void)
{
	BST_fCarSpeed = (BST_s32LeftMotorPulseSigma  + BST_s32RightMotorPulseSigma );// * 0.5 ;//���ҵ��������ƽ��ֵ��ΪС����ǰ����
	BST_s32LeftMotorPulseSigma =BST_s32RightMotorPulseSigma = 0;//ȫ�ֱ��� ע�⼰ʱ����		
	BST_fCarSpeedOld *= 0.7;
	BST_fCarSpeedOld +=BST_fCarSpeed*0.3;
	BST_fCarPosition += BST_fCarSpeedOld;//·��  ���ٶȻ���	   1/11 3:03
	BST_fCarPosition += BST_BluetoothSpeed;
	if(stopflag==1)
	{
		BST_fCarPosition=0;
		
	}
	
	//������������//
	if((s32)BST_fCarPosition > CAR_POSITION_MAX)    BST_fCarPosition = CAR_POSITION_MAX;
	if((s32)BST_fCarPosition < CAR_POSITION_MIN)    BST_fCarPosition = CAR_POSITION_MIN;
																								  
	BST_fSpeedControlOutNew = (BST_fCarSpeedOld -CAR_SPEED_SET ) * BST_fCarSpeed_P + (BST_fCarPosition - CAR_POSITION_SET ) * BST_fCarSpeed_I; //�ٶ�PI�㷨 �ٶ�*P +λ��*I=�ٶ�PWM���
}

void MotorOutput(void) //���PWM�������
{	   
			//�ҵ��ת��PWM�����ں�ƽ��Ƕȡ��ٶ����

	BST_fLeftMotorOut  = BST_fAngleControlOut +BST_fSpeedControlOutNew+BST_fTurnControlOut+BST_BluetoothSpeed;//+directionl - BST_fBluetoothDirectionNew;			//����ת��PWM�����ں�ƽ��Ƕȡ��ٶ����	
  BST_fRightMotorOut = BST_fAngleControlOut +BST_fSpeedControlOutNew-BST_fTurnControlOut+BST_BluetoothSpeed;//-directionl+ BST_fBluetoothDirectionNew;			//�ҵ��ת��PWM�����ں�ƽ��Ƕȡ��ٶ����

		
	if((s16)BST_fLeftMotorOut  > MOTOR_OUT_MAX)	BST_fLeftMotorOut  = MOTOR_OUT_MAX;
	if((s16)BST_fLeftMotorOut  < MOTOR_OUT_MIN)	BST_fLeftMotorOut  = MOTOR_OUT_MIN;
	if((s16)BST_fRightMotorOut > MOTOR_OUT_MAX)	BST_fRightMotorOut = MOTOR_OUT_MAX;
	if((s16)BST_fRightMotorOut < MOTOR_OUT_MIN)	BST_fRightMotorOut = MOTOR_OUT_MIN;
	
  SetMotorVoltageAndDirection((s16)BST_fLeftMotorOut,(s16)BST_fRightMotorOut);
    
}

void SetMotorVoltageAndDirection(s16 s16LeftVoltage,s16 s16RightVoltage)
{
	  u16 u16LeftMotorValue;
	  u16 u16RightMotorValue;
	
    if(s16LeftVoltage<0)//������PWM���Ϊ��ʱ PB14��Ϊ�� PB15��Ϊ�� ��PB14 15 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
    {	
	    GPIO_SetBits(GPIOB, GPIO_Pin_14 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_15 );
      s16LeftVoltage = (-s16LeftVoltage);
    }
    else 
    {	
      GPIO_SetBits(GPIOB, GPIO_Pin_15 );//������PWM���Ϊ��ʱ PB14��Ϊ�� PB15��Ϊ�� ��PB14 15 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
      GPIO_ResetBits(GPIOB, GPIO_Pin_14 ); 
      s16LeftVoltage = s16LeftVoltage;
    }

    if(s16RightVoltage<0)
    {															 //���ҵ��PWM���Ϊ��ʱ PB12��Ϊ�� PB13��Ϊ�� ��PB12 13 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
      GPIO_SetBits(GPIOB, GPIO_Pin_13 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_12 );
      s16RightVoltage = (-s16RightVoltage);
    }
    else														//���ҵ��PWM���Ϊ��ʱ PB12��Ϊ�� PB13��Ϊ�� ��PB12 13 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
    {
	    GPIO_SetBits(GPIOB, GPIO_Pin_12 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_13 );	
     
      s16RightVoltage = s16RightVoltage;
    }
		
	   u16RightMotorValue= (u16)s16RightVoltage;
	   u16LeftMotorValue = (u16)s16LeftVoltage;


	TIM_SetCompare3(TIM2,u16LeftMotorValue);//TIM2�� u16RightMotorValue�Աȣ�����ͬ��ת���Σ�����PWMռ�ձ�
	TIM_SetCompare4(TIM2,u16RightMotorValue);//TIM3�� u16LeftMotorValue�Աȣ�����ͬ��ת���Σ�����PWMռ�ձ�

#if 1	 /*�жϳ��� �Ƿ���ͣ�������������*/
		
  if(Pitch>40||Pitch<-40)
	{		
		TIM_SetCompare3(TIM2,0);
		TIM_SetCompare4(TIM2,0);
		stopflag=1;		
	}
	else stopflag=0;
	
	if(BST_fCarAngle > 50 || BST_fCarAngle < (-50))
	{
		TIM_SetCompare3(TIM2,0);
		TIM_SetCompare4(TIM2,0);  
		stopflag=1;	
	}
	else stopflag=0;

#endif
}

