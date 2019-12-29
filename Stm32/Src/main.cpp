/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "mcnamm_ctl.h"
#include "stdio.h"
#include "message.hpp"

void SystemClock_Config(void);

McNamm_ctl McNammCtl;
Message message;

LeftJoystick LeftJoy;
RightJoystick RightJoy;

void usart1_recv_callback(UART_HandleTypeDef *huart)
{

}

void usart2_recv_callback(UART_HandleTypeDef *huart)
{

}


#define abs(x) (x>0?x:-x)

void HandleMessage(uint8_t msg_id ,uint8_t *buffer ,uint8_t size)
{
	switch(msg_id){
		case MSGID_LEFT_JOYSTICK:
			if(size != sizeof(LeftJoystick)){
				return;
			}
			LeftJoy.move_forward_back = buffer[0];
			LeftJoy.move_left_right = buffer[1];
			printf("forward:%d right:%d\n" , LeftJoy.move_forward_back , LeftJoy.move_left_right);

			break;
		case MSGID_RIGHT_JOYSTICK:
			if(size != sizeof(RightJoystick)){
				return;
			}
			RightJoy.rotate = buffer[0];
			printf("rotate:%d\n" , RightJoy.rotate);
			break;

	}
}

void VelControl()
{
	RotateDir dir = CW; 
	int8_t y_speed = -LeftJoy.move_forward_back; 
	int8_t x_speed = LeftJoy.move_left_right;
	int8_t z_speed = RightJoy.rotate;

	McNammCtl.Kinematic_Analysis(x_speed , y_speed , z_speed);
}

int main(void)
{

	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	MX_UART4_Init();
	MX_USART1_UART_Init(usart1_recv_callback);
	MX_USART2_UART_Init(usart2_recv_callback);
	MX_USART3_UART_Init();
	MX_TIM3_Init();

	LeftJoy.move_forward_back = 0;
	LeftJoy.move_left_right = 0;

	printf("startup\n");
	McNammCtl.McNamm_init();

	while (1)
	{
		if(huart2.RxXferCount > 0)
		{
			//HAL_UART_Transmit_DMA(&huart2 , huart2.pRxBuffPtr , huart2.RxXferSize);
			message.Parse(huart2.pRxBuffPtr , huart2.RxXferCount);
			huart2.RxXferCount = 0;
			
		}

		VelControl();
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
