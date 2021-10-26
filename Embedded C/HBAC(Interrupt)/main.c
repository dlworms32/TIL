2초마다 출력

딜레이를 100ms 주고, it_cnt를 세어 20이 되었을 때 인터럽트를 발생시킴




HVAC2


main.c

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "my_led.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t adc_1_val = 0; // 가변 저항 값
int cur_temp = 0; // 현재 온도
uint16_t humi = 60; // 습도
uint8_t heater = 0; // 히터 가동 상태
uint8_t coolor = 0; // 에어컨 가동 상태
uint16_t cnt = 0;
int _read(int file, char *ptr, int len)
{
	HAL_UART_Receive(&huart2, (uint8_t*)ptr, 1, 0xFFFFFFFF);
	return 1;
}

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)ptr, (uint16_t)len, 0xFFFFFFFF);
	return len;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	adc_1_val = HAL_ADC_GetValue(hadc);
	cur_temp = (adc_1_val / 40) - 20; // -20 ~ 80도
	printf("[%06d] Humi/Temp/Heator/Coolor : %d%% %d %d %d\r\n", cnt++, humi, cur_temp, heater, coolor);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  led_init();
  uint8_t Min_Set = 0; // KEY PA12
  uint8_t Max_Set = 0; // KEY PA11
  uint8_t Up = 0; // KEY PB12
  uint8_t Down = 0; // KEY PB11

  uint16_t Mode = 0; // 1 : Min Set, 2 : Max Set

  uint16_t Max_Temp = 20; // 최대 온도
  uint16_t Min_Temp = 15; // 최소 온도
  uint16_t it_cnt = 0;
  HAL_ADC_Start_IT(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	Min_Set = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
	Max_Set = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	Up = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
	Down = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
	if (!Min_Set || !Max_Set || !Up || !Down) HAL_ADC_Start_IT(&hadc1); // 버튼 입력시 바로 출력
	if (Mode == 0 && !Min_Set){
		printf("Start Min Set : %d\r\n", Min_Temp);
		Mode = 1;
	}
	else if (Mode == 0 && !Max_Set) {
		printf("Start Max Set : %d\r\n", Max_Temp);
		Mode = 2;
	}

	else if (Mode == 1 && !Up && Min_Temp < Max_Temp){
		printf("Min Temp : %d\r\n", ++Min_Temp);
	}
	else if (Mode == 1 && !Down){
		printf("Min Temp : %d\r\n", --Min_Temp);
	}
	else if (Mode == 1 && !Min_Set){
		printf("End Min Set\r\n");
		Mode = 0;
	}

	else if (Mode == 2 && !Up){
		printf("Max Temp : %d\r\n", ++Max_Temp);
	}
	else if (Mode == 2 && !Down && Max_Temp > Min_Temp){
		printf("Max Temp : %d\r\n", --Max_Temp);
	}
	else if (Mode == 2 && !Max_Set){
		printf("End Max Set\r\n");
		Mode = 0;
	}

	if(cur_temp > Max_Temp){
		led_out(LED_1_PIN, TURN_ON);
		led_out(LED_2_PIN, TURN_OFF);
		led_out(LED_3_PIN, TURN_OFF);

		led_out(LED_4_PIN, TURN_OFF); heater = 0;
		led_out(LED_5_PIN, TURN_ON); coolor = 1;
	}
	else if (cur_temp < Min_Temp){
		led_out(LED_1_PIN, TURN_OFF);
		led_out(LED_2_PIN, TURN_ON);
		led_out(LED_3_PIN, TURN_OFF);

		led_out(LED_4_PIN, TURN_ON); heater = 1;
		led_out(LED_5_PIN, TURN_OFF); coolor = 0;
	}
	else {
		led_out(LED_1_PIN, TURN_OFF);
		led_out(LED_2_PIN, TURN_OFF);
		led_out(LED_3_PIN, TURN_ON);

		led_out(LED_4_PIN, TURN_OFF); heater = 0;
		led_out(LED_5_PIN, TURN_OFF); coolor = 0;
	}

	it_cnt++;
	if (it_cnt == 20){
	  HAL_ADC_Start_IT(&hadc1); // 2초마다 인터럽트 호출
	  it_cnt = 0;
	}
	HAL_Delay(100);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/