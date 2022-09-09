/* USER CODE BEGIN Header */
/**
  *************************************
Info:		SHARC buoy compression and encryption
Author:		Tristyn Ferreiro and Shameera Cassim
*************************************
This code compresses and encrypts data on the stm32.

Code is also provided to send data from the STM32 to other devices using UART protocol
by using HAL. You will need Putty or a Python script to read from the serial port on your PC.

In future versions, the data will be read from the sensor HAT ICM2098 chip
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

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

//#define waveHat
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
 uint8_t rxdata[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void pause_sec(float x);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	HAL_UART_Receive_IT(&huart2, rxdata,4); //restart interupt reception
//}
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
  //HAL_UART_Receive_IT(&huart2, rxdata,4);
  float data[][12]={{0.093999997,12,0.999899983,0.0037,-0.0003,0,-0.0018,-0.0024,4.488900185,-0.061000001,-0.061000001,0},
		  {0.123999998,15,0.999899983,0.0035,-0.0004,0,-0.0042,-0.0084,4.700799942,0,-0.061000001,-0.061000001},
		  		  {0.134000003,16,0.999899983,0.0035,-0.0005,0,-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0},
		  		  {0.137999997,17,0.999899983,0.0035,-0.0005,0,-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0},
		  		  {0.145999998,18,0.999899983,0.0035,-0.0005,0,-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0},
		  		  {0.150000006,19,0.999899983,0.0035,-0.0005,0,-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0},
		  		  {0.162,20,0.999899983,0.0034,-0.0006,0,-0.003,-0.0102,4.861299992,-0.061000001,-0.061000001,0},
		  		  {0.165999994,21,0.999899983,0.0034,-0.0006,0,-0.003,-0.0102,4.861299992,-0.061000001,-0.061000001,0},
		  		  {0.175999999,22,0.999899983,0.0034,-0.0006,0,-0.003,-0.0102,4.861299992,-0.061000001,-0.061000001,0},
		  		  {0.185000002,23,0.999899983,0.0033,-0.0007,0,0,-0.0042,4.892399788,-0.061000001,-0.061000001,0},
		  		  {0.188999996,24,0.999899983,0.0033,-0.0007,0,0,-0.0042,4.892399788,-0.061000001,-0.061000001,0},
		  		  {0.206,25,0.999899983,0.0033,-0.0007,0,0,-0.0042,4.892399788,-0.061000001,-0.061000001,0},
		  		  {0.214000002,26,0.999899983,0.0032,-0.0008,1.00E-04,0,-0.006,4.904399872,-0.061000001,-0.061000001,0},
		  		  {0.216999993,27,0.999899983,0.0032,-0.0008,1.00E-04,0,-0.006,4.904399872,-0.061000001,-0.061000001,0},
		  		  {0.223000005,28,0.999899983,0.0032,-0.0008,1.00E-04,0,-0.006,4.904399872,-0.061000001,-0.061000001,0},
		  		  {0.241999999,29,0.999899983,0.0031,-0.001,0,-0.0006,-0.0072,4.893599987,-0.061000001,-0.061000001,0},
		  		  {0.246000007,30,0.999899983,0.0031,-0.001,0,-0.0006,-0.0072,4.893599987,-0.061000001,-0.061000001,0},
		  		  {0.256000012,31,0.999899983,0.0031,-0.001,0,-0.0006,-0.0072,4.893599987,-0.061000001,-0.061000001,0},
		  		  {0.263999999,32,0.999899983,0.003,-0.0011,1.00E-04,0.0012,-0.0066,4.892399788,0,-0.061000001,0}};


  uint8_t header[27];
  sprintf(header, "\r\ncount,AX,AY,AZ,GX,GY,GZ\r\n");
  HAL_UART_Transmit(&huart2, header, sizeof(header), 1000);

  int k =0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	 float accel[3];
	 float gyro[3];

	 while(k<(sizeof(data)/sizeof(data[0]))){
		 pause_sec(5);
		 for(int i =0; i<3;i++){
		 		 accel[i]=data[k][i+2];
		 		 gyro[i]=data[k][i+5];
		 	 }

	     uint8_t msg[55];
	     sprintf(msg, "\r\n%d,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\r\n",(k+1),accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2]);
	     HAL_UART_Transmit(&huart2, msg, sizeof(msg), 1000);

	     k++;
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LD4_Pin|LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void pause_sec(float x)
{
	/* Delay program execution for x seconds */
	//TASK 2

	//declare loop variables
	int i, j, k= 0;

	/*
	 * run through 2 for loops incrementing counter to ensure loop runs (no actual impact on code)
	 * Delay 1 * Delay 2 delays for 1 second.
	 * Multiplying by x gives the delay for x seconds
	 */
	for (i = 0; i < 630; i++) {
		for(j = 0; j < 110*x; j++) { //
			k++; // this is added to ensure that the loop does run
		}
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
