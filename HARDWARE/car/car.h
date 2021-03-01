#ifndef _CAR_H_
#define _CAR_H_

#include "stm32f10x.h"

#define CAR_POSITION_SET (0)

#define MOTOR_OUT_DEAD_VAL       0	   //����ֵ0
#define MOTOR_OUT_MAX           1000	   //ռ�ձ������ֵ
#define MOTOR_OUT_MIN         (-1000)   //ռ�ձȸ����ֵ

#define CAR_POSITION_MAX	8000       //8000
#define CAR_POSITION_MIN	(-8000)     //-8000

extern float CAR_SPEED_SET;
extern float CAR_ZERO_ANGLE;
extern s16 BST_s16LeftMotorPulse;
extern s16 BST_s16RightMotorPulse;

extern s32 BST_s32LeftMotorPulseSigma;
extern s32 BST_s32RightMotorPulseSigma;

extern float BST_fCarSpeed;
extern float BST_fCarSpeedOld;
extern float BST_TurnSpeed;
extern float BST_fCarPosition;
extern float BST_fCarAngle;
extern float BST_fAngleControlOut;
extern float BST_fSpeedControlOut;
extern float BST_fLeftMotorOut;
extern float BST_fRightMotorOut;

extern float BST_u8MainEventCount;
extern float BST_u8SpeedControlCount;
extern float BST_u8SpeedControlPeriod;

/*****************������************************/

extern u8 BST_u8DirectionControlPeriod;
extern u8 BST_u8DirectionControlCount;					  //ת�����ѭ������  ��SysTick_Handler(void)��ʹ�� ÿ5ms��1 
extern u8 BST_u8trig;
extern u8 ucBluetoothValue;                      //������������
extern float volt;



/******������Ʋ���******/
extern float BST_fSpeedControlOut;						   //�ٶȿ���PWM
extern float BST_fSpeedControlOutOld;
extern float BST_fSpeedControlOutNew;
extern float BST_fAngleControlOut;
extern float BST_fTurnControlOut;
extern float BST_fLeftMotorOut;
extern float BST_fRightMotorOut;

extern float BST_fCarAngle;						 //�Ƕȿ���PWM
extern float gyro_z;
extern float gyrx;
extern float gy0;

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*///���²ο�Ϊ�ص���Բο���ͬ��ص�ѹ�йأ������õ��ٵ���
extern float  BST_fCarAngle_P; //����Сʱ�����Ұڣ�����ʱ����  ����������ܹ�վ�� P=91.3�����ڸ�С�����˶�����ʹ��
extern float  BST_fCarAngle_D;	// 0.001 0.002 0.004 0.008 0.0010 0.011	 ��Сʱ��Ӧ��������ʱ�����

extern float  BST_fCarSpeed_P;
extern float  BST_fCarSpeed_I;

extern float BST_fCarTurn_Kd;
extern float BST_fCarTurn_Kp;
extern const double PID_Original[4];
extern char  alldata[80];
extern char *iap;

/******�ٶȿ��Ʋ���******/
extern s16   BST_s16LeftMotorPulse;					  //����������
extern s16	  BST_s16RightMotorPulse;					   //�ҵ��������

extern s32   BST_s32LeftMotorPulseOld;
extern s32   BST_s32RightMotorPulseOld;
extern s32   BST_s32LeftMotorPulseSigma;				  //50ms��������ֵ
extern s32   BST_s32RightMotorPulseSigma;				 //50ms�ҵ������ֵ

extern float BST_fCarSpeed;							 //�������̵ó��ĳ���
extern float BST_fCarSpeedOld;

extern float BST_fCarPosition;						   //��������ͨ������õ���С��λ��

/*-----��ͣ����-----*/
extern int stopflag;
extern int leftstop;
extern int righttstop;

extern void SetMotorVoltageAndDirection(s16 s16LeftVoltage,s16 s16RightVoltage);
extern void GetMotorPulse(void);
extern void AngleControl(void);
extern void MotorOutput(void);
extern void SetMotorVoltageAndDirection(s16 s16LeftVoltage,s16 s16RightVoltage);
extern void SpeedControl(void);
extern void CarUpstandInit(void);
extern void TurnControl(void);

#endif
