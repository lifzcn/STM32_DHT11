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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht11.h"
#include "oled.h"
#include "display.h"
#include "stdio.h"
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
uint8_t rxBuffer[1];		//指令缓存区
#define RX_CMD_LEN 1	//接收指令长度
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint16_t Temperature_Value;//温度采集值
	uint16_t Humidity_Value;//湿度采集值
	
	int Integer_Value_Temp;//温度整数部分
	int Decimal_Value_Temp;//温度小数部分
	int Integer_Value_Humi;//湿度整数部分
	int Decimal_Value_Humi;//湿度小数部分
	
//	uint8_t Deposit_value_1[100];//温度整数暂存
//	uint8_t Deposit_value_2[50];//温度整数暂存
//	uint8_t Deposit_value_3[100];//温度整数暂存
//	uint8_t Deposit_value_4[50];//温度整数暂存
	
//	int i =0;
//	int j =0;
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
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
	
  /* USER CODE BEGIN 2 */
	while(DHT11_Init())
	{
		printf("DHT11接入失败，请重试！\r\n");
		HAL_Delay(500);
	}
	
	OLED_Init();
	OLED_Clear();
	
	OLED_ShowChinese(16,0,0);/*"当",0*/
	OLED_ShowChinese(32,0,1);/*"前",1*/
	OLED_ShowChinese(48,0,2);/*"实",2*/
	OLED_ShowChinese(64,0,3);/*"时",3*/
	OLED_ShowChinese(80,0,4);/*"数",4*/
	OLED_ShowChinese(96,0,5);/*"据",5*/
	
	OLED_ShowChinese(8,4,2);/*"实",7*/
	OLED_ShowChinese(24,4,3);/*"时",8*/
	OLED_ShowChinese(40,4,6);/*"温",0*/
	OLED_ShowChinese(56,4,8);/*"度",2*/
	OLED_ShowChar(72,4,':',16);
	OLED_ShowChinese(8,6,2);/*"实",7*/
	OLED_ShowChinese(24,6,3);/*"时",8*/
	OLED_ShowChinese(40,6,7);/*"湿",1*/
	OLED_ShowChinese(56,6,8);/*"度",2*/
	OLED_ShowChar(72,6,':',16);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	
		HAL_UART_Receive_IT(&huart2,rxBuffer,RX_CMD_LEN);
		
		uint8_t Chr_cmd = rxBuffer[0];
		
		DHT11_Read_Data(&Temperature_Value,&Humidity_Value);
		
		Integer_Value_Temp = Temperature_Value>>8;
		Decimal_Value_Temp = Temperature_Value&0xff;
		
		Integer_Value_Humi = Humidity_Value>>8;
		Decimal_Value_Humi = Humidity_Value&0xff;
		
		printf("==========================\r\n");
		printf("实时温度为：%d.%dC\r\n",Integer_Value_Temp,Decimal_Value_Temp);
		printf("实时湿度为：%d.%d%%\r\n",Integer_Value_Humi,Decimal_Value_Humi);
		printf("==========================\r\n");
		
//		Deposit_value_1[i] = (int)Integer_Value_Temp/10;
//		Deposit_value_1[i+1] = (int)Integer_Value_Temp%10;
//		
//		Deposit_value_2[i] = (int)Decimal_Value_Temp;
//		
//		Deposit_value_3[i] = (int)Integer_Value_Humi/10;
//		Deposit_value_3[i+1] = (int)Integer_Value_Humi%10;
//		
//		Deposit_value_4[i] = (int)Decimal_Value_Humi;
		
		switch(Chr_cmd)
		{
			case '1':
				Display_Normal(Integer_Value_Temp,Decimal_Value_Temp,Integer_Value_Humi,Decimal_Value_Humi);
			break;
		
			case '2':
				OLED_Init();
				OLED_Clear();
			
				OLED_WR_CMD(0x2E);
				OLED_WR_CMD(0xA3);
				OLED_WR_CMD(0x00);
				OLED_WR_CMD(0x40);
				OLED_WR_CMD(0x2A);
				OLED_WR_CMD(0x00);
				OLED_WR_CMD(0x00);
				OLED_WR_CMD(0x01);
				OLED_WR_CMD(0x00);
				OLED_WR_CMD(0x04);
	
//				OLED_ShowString(12,4,"Temp",16);
//				OLED_ShowChar(44,4,':',16);
//				
//				OLED_ShowString(12,6,"Humi",16);
//				OLED_ShowChar(44,6,':',16);
//			
//				OLED_ShowNum(52,4,Integer_Value_Temp,2,16);
//				OLED_ShowChar(68,4,'.',16);
//				OLED_ShowNum(76,4,Decimal_Value_Temp,1,16);
//				OLED_ShowChar(84,4,'C',16);
//				OLED_ShowNum(52,6,Integer_Value_Humi,2,16);
//				OLED_ShowChar(68,6,'.',16);
//				OLED_ShowNum(76,6,Decimal_Value_Humi,1,16);
//				OLED_ShowChar(84,6,'%',16);
			
				OLED_ShowChinese(8,2,2);/*"实",7*/
				OLED_ShowChinese(24,2,3);/*"时",8*/
				OLED_ShowChinese(40,2,6);/*"温",0*/
				OLED_ShowChinese(56,2,8);/*"度",2*/
				OLED_ShowChar(72,2,':',16);
				OLED_ShowChinese(8,6,2);/*"实",7*/
				OLED_ShowChinese(24,6,3);/*"时",8*/
				OLED_ShowChinese(40,6,7);/*"湿",1*/
				OLED_ShowChinese(56,6,8);/*"度",2*/
				OLED_ShowChar(72,6,':',16);
	
				OLED_ShowNum(80,2,Integer_Value_Temp,2,16);
				OLED_ShowChar(96,2,'.',16);
				OLED_ShowNum(104,2,Decimal_Value_Temp,1,16);
				OLED_ShowChar(112,2,'C',16);
				OLED_ShowNum(80,6,Integer_Value_Humi,2,16);
				OLED_ShowChar(96,6,'.',16);
				OLED_ShowNum(104,6,Decimal_Value_Humi,1,16);
				OLED_ShowChar(112,6,'%',16);
	
//				OLED_ShowChar(72,4,Deposit_value_1[j],16);
//				OLED_ShowChar(80,4,Deposit_value_1[j+1],16);
//				OLED_ShowChar(88,4,'.',16);
//				OLED_ShowChar(96,4,Deposit_value_2[j],16);
//				OLED_ShowChar(104,4,'C',16);
//				OLED_ShowChar(72,6,Deposit_value_3[j],16);
//				OLED_ShowChar(80,6,Deposit_value_3[j+1],16);
//				OLED_ShowChar(88,6,'.',16);
//				OLED_ShowChar(96,4,Deposit_value_4[j],16);
//				OLED_ShowChar(104,6,'%',16);
	
				OLED_WR_CMD(0x2F);//开启滚动
				
				HAL_Delay(6000);
			break;
		
			default:
				Display_Normal(Integer_Value_Temp,Decimal_Value_Temp,Integer_Value_Humi,Decimal_Value_Humi);
			break;
		}
		
		HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		HAL_UART_Receive_IT(huart,rxBuffer,RX_CMD_LEN);//再次接收
	}
}

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
