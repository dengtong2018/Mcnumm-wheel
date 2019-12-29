/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define PASET(pin) HAL_GPIO_WritePin(GPIOA , pin , GPIO_PIN_SET);
#define PARESET(pin) HAL_GPIO_WritePin(GPIOA , pin , GPIO_PIN_RESET);
#define PBSET(pin) HAL_GPIO_WritePin(GPIOB , pin , GPIO_PIN_SET);
#define PBRESET(pin) HAL_GPIO_WritePin(GPIOB , pin , GPIO_PIN_RESET);
#define PCSET(pin) HAL_GPIO_WritePin(GPIOC , pin , GPIO_PIN_SET);
#define PCRESET(pin) HAL_GPIO_WritePin(GPIOC , pin , GPIO_PIN_RESET);
#define PDSET(pin) HAL_GPIO_WritePin(GPIOD , pin , GPIO_PIN_SET);
#define PDRESET(pin) HAL_GPIO_WritePin(GPIOD , pin , GPIO_PIN_RESET);
#define PESET(pin) HAL_GPIO_WritePin(GPIOE , pin , GPIO_PIN_SET);
#define PERESET(pin) HAL_GPIO_WritePin(GPIOE , pin , GPIO_PIN_RESET);

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
