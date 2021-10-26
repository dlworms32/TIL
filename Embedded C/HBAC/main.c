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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
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
UART_HandleTypeDef huart2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t Min_Set = 0; // PA10
  uint8_t Max_Set = 0; // PB3
  uint8_t Up = 0; // PB5
  uint8_t Down = 0; // PB4
  uint8_t Heater = 0;
  uint8_t Coolor = 0;
  uint16_t Mode = 0; // 1 : Min Set, 2 : Max Set
  uint16_t Cur_Temp = 20;  // 현재 온도

  uint16_t Max_Temp = 20; // 최대 온도
  uint16_t Min_Temp = 15; // 최소 온도

  uint16_t cnt = 0;
  uint16_t Humi = 50;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Min_Set = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
	  if (Min_Set){
		  if (Mode == 0){
			  printf("Min_Set Mode\r\n");
			  printf("Before Min_Temp : %d\r\n", Min_Temp);
			  Mode = 1;
			  HAL_Delay(1000);
		  }
		  else if (Mode == 1){
			  printf("End Min_Set Mode\r\n");
			  printf("Set Min_Temp : %d\r\n", Min_Temp);
			  Mode = 0;
			  HAL_Delay(1000);
		  }
	  }
	  Max_Set = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	  if (Max_Set){
		  if (Mode == 0){
			  printf("Max_Set Mode\r\n");
			  printf("Before Max_Temp : %d\r\n", Max_Temp);
			  Mode = 2;
			  HAL_Delay(1000);
		  }
		  else if (Mode == 2){
			  printf("End Min_Set Mode\r\n");
			  printf("Set Max_Temp : %d\r\n", Max_Temp);
			  Mode = 0;
			  HAL_Delay(1000);
		  }
	  }
	  Up = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	  if (Up){
		  if (Mode == 1){
			  if (Min_Temp < Max_Temp - 1){
				  printf("Min_Temp : %d\r\n", ++Min_Temp);
			  }
			  else {
				  printf("Min Temp Set error\r\n");
			  }
			  HAL_Delay(1000);
		  }
		  else if (Mode == 2){
			  printf("Max_Temp : %d\r\n", ++Max_Temp);
			  HAL_Delay(1000);
		  }
		  else{
			  Cur_Temp++;
			  printf("Current temperature : %d\r\n", Cur_Temp);
			  HAL_Delay(500);
		  }
	  }
	  Down = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	  if (Down){
		  if (Mode == 1){
			  printf("Min_Temp : %d\r\n", --Min_Temp);
			  HAL_Delay(1000);
		  }
		  else if (Mode == 2){
			  if (Max_Temp > Min_Temp + 1){
				  printf("Max_Temp : %d\r\n", --Max_Temp);
			  }
			  else {
				  printf("Max Temp Set error\r\n");
			  }
			  HAL_Delay(1000);
		  }
		  else {
			  Cur_Temp--;
			  printf("Current temperature : %d\r\n", Cur_Temp);
			  HAL_Delay(500);
		  }
	  }

	  if(Cur_Temp > Max_Temp){
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); // Green
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // Yellow
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Red

		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // 히터
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // 에어컨
		  Coolor = 1;
		  Heater = 0;
	  }
	  else if(Cur_Temp < Min_Temp){
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);  // Green
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);  // Yellow
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET); // Red

		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // 히터
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // 에어컨
		  Coolor = 0;
		  Heater = 1;
	  }
	  else {
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);  // Green
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  // Yellow
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  // Red

		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // 히터
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // 에어컨
		  Coolor = 0;
		  Heater = 0;
	  }
	  if (Mode == 0){
		  printf("[%06d] Humi/Temp/Heator/Coolor : %d%%/%d/%d/%d\r\n", cnt++, Humi, Cur_Temp, Heater, Coolor);
		  HAL_Delay(500);
	  }
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
