/* USER CODE BEGIN Header */
/**
*************************************
Info:		STM32 I2C with DS3231 HAL
Author:		Amaan Vally
*************************************
In this practical you will learn to use I2C on the STM32 using the HAL. Here, we will
be interfacing with a DS3231 RTC. We also create functions to convert the data between Binary
Coded Decimal (BCD) and decimal.

Code is also provided to send data from the STM32 to other devices using UART protocol
by using HAL. You will need Putty or a Python script to read from the serial port on your PC.

UART Connections are as follows: red->5V black->GND white(TX)->PA2 green(RX;unused)->PA3.
Open device manager and go to Ports. Plug in the USB connector with the STM powered on. Check the port number (COMx).
Open up Putty and create a new Serial session on that COMx with baud rate of 9600.

https://www.youtube.com/watch?v=EEsI9MxndbU&list=PLfIJKC1ud8ghc4eFhI84z_3p3Ap2MCMV-&index=4

RTC Connections: (+)->5V (-)->GND D->PB7 (I2C1_SDA) C->PB6 (I2C1_SCL)
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
typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} TIME;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//TO DO:
//TASK 2
//Give DELAY1 and DELAY2 sensible values
#define DELAY1 630
#define DELAY2 110

//TO DO:
//TASK 4
//Define the RTC slave address
#define DS3231_ADDRESS 0xD0 //0b1101000
#define REG_SIZE 0x01
#define FIRST_REG 0x00

#define EPOCH_2022 1640988000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
char buffer[14];
uint8_t data [] = "Hello from STM32!\r\n";
TIME time;
char tmpRtc[31]; // this is a string used for formating RTC time to print to screen
int e = 3;
int n = 187;
int d = 107;
int p = 11;
int q = 17;
int encryptedBits = 0;
int encryptedData[20];

//for timing
int start = 0;
int end = 0;
int t = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UART_TxCpltCllback(UART_HandleTypeDef *huart);
void pause_sec(float x);

uint8_t decToBcd(int val);
int bcdToDec(uint8_t val);
void setTime (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);
void getTime (void);
int epochFromTime(TIME time);
int ENCmodpow(int base, int power, int mod);
void encrypt(char msg[]);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void){

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  setTime(0,0,19,2,4,10,22); //set the date to something random.


  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */


  //TO DO
  //TASK 6
  char inputArray[] = {"10.054000001,6,0.0024,-0.0006,3.856600046,-0.061000001,-0.061000001,0,34.83589935\n0.066,7,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n0.07,8,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n0.082999997,9,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n0.085000001,10,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n0.101999998,11,-0.0006,-0.0048,4.633200169,-0.061000001,-0.061000001,0,34.81240082\n0.109999999,12,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n0.112999998,13,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n0.131999999,14,0.003,-0.003,4.794199944,0,-0.061000001,0,34.83000183\n0.140000001,15,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.143999994,16,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.156000003,17,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.164000005,18,0,-0.006,4.858300209,-0.061000001,-0.061000001,0,34.81240082\n0.172999993,19,-0.003,-0.0054,4.869699955,-0.061000001,-0.061000001,0,34.81240082}"};

  //This displays the header which explains the formating of the data outputed.
  uint8_t header2[81];
  sprintf(header2, "\r\nAccel X (g),Accel Y (g),Accel Z (g),Gyro X (dps),Gyro Y (dps),Gyro Z (dps),Temp");
  HAL_UART_Transmit(&huart2, header2, sizeof(header2), 1000);

	 int run = 0;

  /* USER CODE END 2 */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */


    /* USER CODE BEGIN 3 */
	  if (run ==0) {
		  	  getTime();
		  	  start = epochFromTime(time);
	  		  encrypt(inputArray);
	  		  getTime();
	  		  end = epochFromTime(time);
	  		  t = end-start;
	  		  //memcpy(encryptedData, &encrypted, sizeof(encrypted)+1);
	  	       int count = 0;
	  	       while (count < encryptedBits) {
	  			  char temp[7];
	  			  sprintf(temp, "\r\n%d, ",encryptedData[count]);
	  			  HAL_UART_Transmit(&huart2, temp, sizeof(temp), 1000);
	  			  count++;
	  		  }
	  		  char temp2[22];
	  		  sprintf(temp2, "\r\ntime taken:%d",t);
	  		  HAL_UART_Transmit(&huart2, temp2, sizeof(temp2), 1000);
	  		  //char temp2[22];
	  		  //sprintf(temp2, "\r\ntime taken:%d",end);
	  		  //HAL_UART_Transmit(&huart2, temp2, sizeof(temp2), 1000);
	  		  run++;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
	for (i = 0; i < DELAY1; i++) {
		for(j = 0; j < DELAY2*x; j++) { //
			k++; // this is added to ensure that the loop does run
		}
	}
}

uint8_t decToBcd(int val)
{
    /* Convert normal decimal numbers to binary coded decimal*/
	//TASK 3

	/* calculate the BCD value by shifting the decimal value right 4 bits, multiplying
	 * that result by 10 and adding that to the dec value anded with 1111(binary (= 0F in hex)) */
    uint8_t bcd = ((val/10) <<4)+ (val % 10);
	return bcd;

}

int bcdToDec(uint8_t val)
{
    /* Convert binary coded decimal to normal decimal numbers */
	//TASK 3

	/* BCD to DEC calculation explained:
	 * calculate the decimal value from the BCD value by dividing the dec value by 10 and
	 * shifting the result 4 bits left. The result of this is added to the value of the BCD
	 * value mod 10 (remainder when dividing by 10)
	 */
	uint8_t dec = (val>>4) * 10 + (val & 0x0F);
    return dec;

}

void setTime (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
    /* Write the time to the RTC using I2C */
	//TASK 4
    uint8_t set_time[7]; // create the array to store the time in

    // fill the array with the values given via function arguments
    // values must be converted from decimal to BCD
    set_time[0]=decToBcd(sec);
    set_time[1]=decToBcd(min);
    set_time[2]=decToBcd(hour);
    set_time[3]=decToBcd(dow);
    set_time[4]=decToBcd(dom);
    set_time[5]=decToBcd(month);
    set_time[6]=decToBcd(year);

    // Write the array storing time to memory
    // DS3231_ADDRES is defined in constants as 0xD0, REG_SIZE is defined as 1,  FIRST_REG is defined as 0
    HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, FIRST_REG, REG_SIZE, set_time, 7, 1000);

}

void getTime (void)
{
    /* Get the time from the RTC using I2C */
	//TASK 4
	uint8_t get_time[7]; // array to store RTC time in

	//Read the time from memory
	//DS3231_ADDRES is defined in constants as 0xD0, REG_SIZE is defined as 1,  FIRST_REG is defined as 0
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, FIRST_REG, REG_SIZE, get_time, 7, 1000);

	//Update the TIME structure using the values from the array above
	//values must be converted from BCD to decimal
	time.seconds=bcdToDec(get_time[0]);
	time.minutes=bcdToDec(get_time[1]);
	time.hour=bcdToDec(get_time[2]);
	time.dayofweek=bcdToDec(get_time[3]);
	time.dayofmonth=bcdToDec(get_time[4]);
	time.month=bcdToDec(get_time[5]);
	time.year=bcdToDec(get_time[6]);

}

int epochFromTime(TIME time){
    /* Convert time to UNIX epoch time (leap years are ignored and 2022 is used as reference)*/
	//TASK 5

	//extract the individual parts of the time structure
	uint8_t s = time.seconds;
	uint8_t min = time.minutes;
	uint8_t h = time.hour;
	uint8_t dow = time.dayofweek;
	uint8_t dom = time.dayofmonth;
	uint8_t month = time.month;
	uint8_t year = time.year;

	//add the day of the month to total days
	uint8_t totDays = dom;

	/*
	 * Switch statement to add the number of days in between a day
	 */
	//add number of days for all the months before that month
	switch(month){
		case 1:
			break;
		case 2:
			totDays += 31;
			break;
		case 3:
			totDays += (31+28);
			break;
		case 4:
			totDays += (31*2+28);
			break;
		case 5:
			totDays += (31*2+28+30);
			break;
		case 6:
			totDays += (31*3+28+30);
			break;
		case 7:
			totDays += (31*3+28+30*2);
			break;
		case 8:
			totDays += (31*4+28+30*2);
			break;
		case 9:
			totDays += (31*5+28+30*2);
			break;
		case 10:
			totDays += (31*5+28+30*3);
			break;
		case 11:
			totDays += (31*6+28+30*3);
			break;
	    case 12:
	    	totDays += (31*6+28+30*4);
	    	break;
		default:
			totDays = totDays;
	}

	//add up the total seconds from Saturday, January 1, 2022 12:00:00 AM GMT+02:00 until given time
	//there are 31536000 seconds in a year, 86400 seconds in a day, 3600 seconds in an hour and 60
	//seconds in a minute
	int totSec = (year-22)*31536000 + (totDays*86400) + (h*3600) + (min*60) + s;

	/* return the sum of the epoch time on Saturday, January 1, 2022 12:00:00 AM GMT+02:00 and the total
	 * seconds after that */

	return (EPOCH_2022 + totSec) ;
}

void rtcPrint(TIME time) {
	/* Format RTC time to nice string representation for UART transmission*/
	// Task 6

	//extract the individual parts of the time structure
	uint8_t s = time.seconds;
	uint8_t min = time.minutes;
	uint8_t h = time.hour;
	uint8_t dow = time.dayofweek;
	uint8_t dom = time.dayofmonth;
	uint8_t month = time.month;
	uint8_t year = time.year;

	/*
	 * tmpRtc holds the formatted RTC time to be transmitted over UART and printed to screen.
	 *
	 * Switch statement formats the RTC time with correct day of week.
	 */
	switch(dow){
		case 1: {
			sprintf(tmpRtc, "Monday, %02d/%02d/%02d, %02d:%02d:%02d    \n", dom, month, year, h, min, s);
			break;
		}
		case 2: {
			sprintf(tmpRtc, "Tuesday, %02d/%02d/%02d, %02d:%02d:%02d   \n", dom, month, year, h, min, s);
			break;
		}
		case 3: {
			sprintf(tmpRtc, "Wednesday, %02d/%02d/%02d, %02d:%02d:%02d \n", dom, month, year, h, min, s);
			break;
		}
		case 4: {
			sprintf(tmpRtc, "Thursday, %02d/%02d/%02d, %02d:%02d:%02d  \n", dom, month, year, h, min, s);
			break;
		}
		case 5: {
			sprintf(tmpRtc, "Friday, %02d/%02d/%02d, %02d:%02d:%02d    \n", dom, month, year, h, min, s);
			break;
		}
		case 6: {
			sprintf(tmpRtc, "Saturday, %02d/%02d/%02d, %02d:%02d:%02d  \n", dom, month, year, h, min, s);
			break;
		}
		case 7: {
			sprintf(tmpRtc, "Sunday, %02d/%02d/%02d, %02d:%02d:%02d    \n", dom, month, year, h, min, s);
			break;
		}
		default: {
			sprintf(tmpRtc, "%02d/%02d/%02d, %02d:%02d:%02d            \n", dom, month, year, h, min, s);
		}
	}
	return;
}

int ENCmodpow(int base, int power, int mod)
{
       int i;
       int result = 1;
       for (i = 0; i < power; i++)
       {
               result = (result * base) % mod;
       }
       return result;
}

void encrypt(char msg[]) {
	int c;
	int i;

       for (i = 0; msg[i]!= '}'; i++)
       {
           c = ENCmodpow(msg[i],e,n);
           encryptedData[i] = c;
           encryptedBits++;
          // if (i > 0) {
          // sprintf(mesg, "%d and i-1 =%dP",encryptedData[i], encryptedData[i-1]);
          //  HAL_UART_Transmit(&huart2, mesg, sizeof(mesg), 1000);
          // }
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
