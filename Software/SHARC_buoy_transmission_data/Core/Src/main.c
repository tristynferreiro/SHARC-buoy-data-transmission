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
#include "stdlib.h"
#include "string.h"
#include "time.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* FOR COMPRESSION
#define EI 11  // typically 10..13
#define EJ  5  // typically 4..5
#define P   1  //If match length <= P then output one character
#define N (1 << EI)  // buffer size
#define F ((1 << EJ) + 1)  // lookahead buffer size
*/

/* FOR ENCRYPTION */
#define MAX_VALUE 32
#define E_VALUE 3 /*65535*/

//#define waveHat
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
 /*uint8_t rxdata[4];
 int numRecordings =0; // this keeps track of the number of recordings.

 int bit_buffer = 0, bit_mask = 128;
 unsigned long codecount = 0, textcount = 0;
 unsigned char buffer[N * 2]; */
 // needs to be at least the size of the input data (minimum). this size should be the limit of data stored at any one time
/* char compressed[4970000];
 int compressedBits =0;
*/
 char inputData [200];

 int encryptedData[200];
 int encryptedBits = 0;
 int e = E_VALUE;
 int n = 667;
 int d = 411;
 int p = 23;
 int q = 29;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
/*void pause_sec(float x);
void store(int bitbuffer);
void putbit1(void);
void flush_bit_buffer(void);
void output1(int c);
void output2(int x, int y);
void encode(char inputData[]); */
int ENCmodpow(int base, int power, int mod);
void encrypt(char msg[]);


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

  char inputArray[] = {"13, 14, 15}"};

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

	/*
	float accel[3];
	float gyro[3];
	float temp;

	while(numRecordings<(sizeof(data)/sizeof(data[0]))){
		pause_sec(2); //delay is implemented

		for(int i =0; i<3;i++){
		 		accel[i]=data[numRecordings][i];
		 		gyro[i]=data[numRecordings][i+3];
		}
		temp=data[numRecordings][6];

	    uint8_t msg[100];
	 */
	  /*
	  	      * FORMATTING BREAK DOWN:
	  	      * 0: the reading number (k-value)
	  	      *
	  	      * 94-100: spaces added at the end as buffer in case additional characters (like negative signs) appear
	  	      */
	  /*
	     sprintf(msg, "\r\n%.9f,%.9f,%.9f,%.9f,%.9f,%.9f,%.9f         ",accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2],temp);
	     //sprintf(msg, "\r\n%.4f,%.4f,%.9f,%.9f,%.9f,%.9f,%.9f         ",accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2],temp);

	  */

	 /* encode(inputArray);

	  uint8_t msg[compressedBits];
	  sprintf(msg, "%s",compressed);
    */
	  //HAL_UART_Transmit(&huart2, header2, sizeof(header2), 1000);

	  if (run ==0) {
		  encrypt(inputArray);
		  //memcpy(encryptedData, &encrypted, sizeof(encrypted)+1);
		  int count = 0;
		  while (count < encryptedBits) {
			  char temp[4];
			  sprintf(temp, "%d, ",encryptedData[count]);
			  HAL_UART_Transmit(&huart2, temp, sizeof(temp), 1000);
			  count++;
		  }
		  run++;
	  }


	  //numRecordings++; //used for determining what size the compressed array should be set to
  }


  }
  /* USER CODE END 3 */


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

	//declare loop variables
	int i, j, k= 0;

	/*
	 * run through 2 for loops incrementing counter to ensure loop runs (no actual impact on code)
	 * Multiplying by x gives the delay for x seconds
	 */
	for (i = 0; i < 630; i++) {
		for(j = 0; j < 110*x; j++) { //
			k++; // this is added to ensure that the loop does run
		}
	}
}
/********************************
 * THIS IS THE COMPRESSION CODE
 *******************************/
/*void store(int bitbuffer){
    compressed[compressedBits]=bitbuffer;
    compressedBits++;
}

void putbit1(void)
{
    bit_buffer |= bit_mask;
    if ((bit_mask >>= 1) == 0) {
        store(bit_buffer);
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void putbit0(void)
{
    if ((bit_mask >>= 1) == 0) {
        store(bit_buffer);
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void flush_bit_buffer(void)
{
    if (bit_mask != 128) {
        store(bit_buffer);
        codecount++;
    }
}


void output1(int c)
{
    int mask;

    putbit1();
    mask = 256;
    while (mask >>= 1) {
        if (c & mask) putbit1();
        else putbit0();
    }
}

void output2(int x, int y)
{
    int mask;

    putbit0();
    mask = N;
    while (mask >>= 1) {
        if (x & mask) putbit1();
        else putbit0();
    }
    mask = (1 << EJ);
    while (mask >>= 1) {
        if (y & mask) putbit1();
        else putbit0();
    }
}

void encode(char inputData[])
{
    int i, j, f1, x, y, r, s, bufferend, c;

    int counter = 0;
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    for (i = N - F; i < N * 2; i++) {
        if ( counter >= sizeof(inputData)) break;
        c = inputData[counter];
        buffer[i] = c;  counter++;
    }
    bufferend = i;  r = N - F;  s = 0;
    while (r < bufferend) {
        f1 = (F <= bufferend - r) ? F : bufferend - r;
        x = 0;  y = 1;  c = buffer[r];
        for (i = r - 1; i >= s; i--)
            if (buffer[i] == c) {
                for (j = 1; j < f1; j++)
                    if (buffer[i + j] != buffer[r + j]) break;
                if (j > y) {
                    x = i;  y = j;
                }
            }
        if (y <= P) {  y = 1;  output1(c);  }
        else output2(x & (N - 1), y - 2);
        r += y;  s += y;
        if (r >= N * 2 - F) {
            for (i = 0; i < N; i++) buffer[i] = buffer[i + N];
            bufferend -= N;  r -= N;  s -= N;
            while (bufferend < N * 2) {
                if ( counter >= sizeof(inputData)) break;
                c = inputData[counter];
                buffer[bufferend++] = c;  counter++;
            }
        }
    }
    flush_bit_buffer();
}
*/

/********************************
 * THIS IS THE ENCRYPTION CODE
 *******************************/
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
            //int mesg[4];
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
