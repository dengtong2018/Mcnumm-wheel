#ifndef __MCNAMM_CTL_H__
#define __MCNAMM_CTL_H__

#include "gpio.h"
#include "tim.h"

#define a_PARAMETER 0.5
#define b_PARAMETER 0.5

typedef enum 
{
	WHEEL_1,
	WHEEL_2,
	WHEEL_3,
	WHEEL_4,
	WHEEL_MAX
}WheelNum;

typedef struct
{
	TIM_HandleTypeDef *timer_num;
	uint32_t pwm_channel;
	GPIO_TypeDef *direc_gpio1;
	uint16_t direc_pin1;
	GPIO_TypeDef *direc_gpio2;
	uint16_t direc_pin2;
}WheelIo;

typedef enum 
{
	CW, //顺时针
	CCW //逆时针
}RotateDir;

extern WheelIo wheel_io[WHEEL_MAX];

class McNamm_ctl
{
	public:
		McNamm_ctl();
		void McNamm_init();
		void change_direction(WheelNum , RotateDir);//改变旋转方向 0顺时针，1逆时针
		void single_wheel_speed(WheelNum , uint16_t);//设置单个轮子的速度
		void Kinematic_Analysis(float Vx,float Vy,float Vz);
		void Wheel_Ctl(float a , float b , float c , float d);

	private:
};

#endif //__MCNAMM_CTL_H__

