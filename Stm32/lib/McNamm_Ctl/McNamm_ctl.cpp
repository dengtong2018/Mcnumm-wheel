#include "mcnamm_ctl.h"
#include "stdio.h"
WheelIo wheel_io[WHEEL_MAX] =
{	
	{&htim3 , TIM_CHANNEL_1 , GPIOB , GPIO_PIN_9 , GPIOE , GPIO_PIN_0},
	{&htim3 , TIM_CHANNEL_2 , GPIOE , GPIO_PIN_2 , GPIOE , GPIO_PIN_1},
	{&htim3 , TIM_CHANNEL_3 , GPIOE , GPIO_PIN_3 , GPIOE , GPIO_PIN_4},
	{&htim3 , TIM_CHANNEL_4 , GPIOE , GPIO_PIN_6 , GPIOE , GPIO_PIN_5}	
};

McNamm_ctl::McNamm_ctl()
{

}

void McNamm_ctl::McNamm_init()
{
	for(uint8_t i = 0 ; i < WHEEL_MAX ; i++)
	{
 		HAL_TIM_PWM_Start(wheel_io[i].timer_num , wheel_io[i].pwm_channel);
	}
}

void McNamm_ctl::single_wheel_speed(WheelNum num , uint16_t speed)
{
	speed = speed > 100 ? 100 : speed;
	uint16_t speed_temp = speed/100.0*1000.0;

	__HAL_TIM_SetCompare(wheel_io[num].timer_num, wheel_io[num].pwm_channel , speed_temp);
}

//改变旋转方向 0顺时针，1逆时针
void McNamm_ctl::change_direction(WheelNum num , RotateDir rotate_dir)
{
	if(num >= WHEEL_MAX)
	{
		return;
	}

	if(CW == rotate_dir)
	{
		HAL_GPIO_WritePin(wheel_io[num].direc_gpio1 , wheel_io[num].direc_pin1, GPIO_PIN_RESET);
 		HAL_GPIO_WritePin(wheel_io[num].direc_gpio2 , wheel_io[num].direc_pin2, GPIO_PIN_SET);
	}
	else if(CCW == rotate_dir)
	{
		HAL_GPIO_WritePin(wheel_io[num].direc_gpio1 , wheel_io[num].direc_pin1, GPIO_PIN_SET);
 		HAL_GPIO_WritePin(wheel_io[num].direc_gpio2 , wheel_io[num].direc_pin2, GPIO_PIN_RESET);
	}
}

#define abs(x) (x>0?x:-x)

void McNamm_ctl::Wheel_Ctl(float a , float b , float c , float d)
{
	RotateDir dirA = (a >= 0 ? CW : CCW);
	RotateDir dirB = (b >= 0 ? CW : CCW);
	RotateDir dirC = (c >= 0 ? CW : CCW);
	RotateDir dirD = (d >= 0 ? CW : CCW);

	uint8_t speed_a = abs(a); 
	uint8_t speed_b = abs(b); 
	uint8_t speed_c = abs(c); 
	uint8_t speed_d = abs(d); 

	change_direction(WHEEL_1, dirA);
	change_direction(WHEEL_2, dirB);
	change_direction(WHEEL_3, dirC);
	change_direction(WHEEL_4, dirD);
	//printf("A:%d %d %d %d\n" , dirA ,dirB ,dirC ,dirD);

	single_wheel_speed(WHEEL_1, speed_a);
	single_wheel_speed(WHEEL_2, speed_b);
	single_wheel_speed(WHEEL_3, speed_c);
	single_wheel_speed(WHEEL_4, speed_d);
}

void McNamm_ctl::Kinematic_Analysis(float Vx,float Vy,float Vz)
{
	Vx = Vx/100.0*50.0;
	Vy = Vy/100.0*50.0;
	Vz = Vz/100.0*50.0;

	float Target_A , Target_B ,Target_C ,Target_D;
    Target_A = -Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
    Target_B = +Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
    Target_C = -Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
    Target_D = +Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
	//printf("A:%f %f %f %f\n" , Target_A ,Target_B ,Target_C ,Target_D);
	Wheel_Ctl(Target_A , Target_B , Target_C  , Target_D);
}
