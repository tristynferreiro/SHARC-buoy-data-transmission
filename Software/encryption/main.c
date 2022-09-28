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
#include "inttypes.h"
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
#define MAX_VALUE 65535
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

 char encryptedData[200];
 int encryptedBits = 0;
 uint16_t e = E_VALUE;
 uint32_t n = 15366391;
 uint32_t d = 10239035;
 uint16_t p = 3917;
 uint16_t q = 3923;



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
unsigned long long int ENCmodpow(int base, int power, int mod);
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
  //HAL_UART_Receive_IT(&huart2, rxdata,4);

  // data is used to imitate the data that will be gathered from the sense HAT in future.
  /*
  float data[][13]={{-0.0018,-0.0024,4.488900185,-0.061000001,-0.061000001,0,30.37709999},
		  {-0.0042,-0.0084,4.700799942,0,-0.061000001,-0.061000001,30.3946991},
		  {-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0,30.38879967},
		  {-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0,30.38879967},
		  {-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0,30.38879967},
		  {-0.0036,-0.0108,4.804399967,-0.061000001,-0.061000001,0,30.38879967},
		  {-0.003,-0.0102,4.861299992,-0.061000001,-0.061000001,0,30.38290024},
		  {-0.003,-0.0102,4.861299992,-0.061000001,-0.061000001,0,30.38290024},
		  {-0.003,-0.0102,4.861299992,-0.061000001,-0.061000001,0,30.38290024},
		  {0,-0.0042,4.892399788,-0.061000001,-0.061000001,0,30.37120056},
		  {0,-0.0042,4.892399788,-0.061000001,-0.061000001,0,30.37120056},
		  {0,-0.0042,4.892399788,-0.061000001,-0.061000001,0,30.37120056},
		  {0,-0.006,4.904399872,-0.061000001,-0.061000001,0,30.37709999},
		  {0,-0.006,4.904399872,-0.061000001,-0.061000001,0,30.37709999},
		  {0,-0.006,4.904399872,-0.061000001,-0.061000001,0,30.37709999},
		  {-0.0006,-0.0072,4.893599987,-0.061000001,-0.061000001,0,30.36529922},
		  {-0.0006,-0.0072,4.893599987,-0.061000001,-0.061000001,0,30.36529922},
		  {-0.0006,-0.0072,4.893599987,-0.061000001,-0.061000001,0,30.36529922},
		  {0.0012,-0.0066,4.892399788,0,-0.061000001,0,30.36529922},
		  {0.0012,-0.0066,4.892399788,0,-0.061000001,0,30.36529922},
		  {0.0012,-0.0066,4.892399788,0,-0.061000001,0,30.36529922},
		  {0.0036,-0.003,4.907400131,-0.061000001,-0.061000001,-0.061000001,30.37120056},
		  {0.0036,-0.003,4.907400131,-0.061000001,-0.061000001,-0.061000001,30.37120056},
		  {0.0036,-0.003,4.907400131,-0.061000001,-0.061000001,-0.061000001,30.37120056},
		  {0.0048,-0.0018,4.914000034,-0.061000001,-0.061000001,0,30.3593998},
		  {0.0048,-0.0018,4.914000034,-0.061000001,-0.061000001,0,30.3593998},
		  {0.0048,-0.0018,4.914000034,-0.061000001,-0.061000001,0,30.3593998},
		  {0.0042,-0.0066,4.902599812,0,-0.061000001,-0.061000001,30.34760094},
		  {0.0042,-0.0066,4.902599812,0,-0.061000001,-0.061000001,30.34760094},
		  {0.0042,-0.0066,4.902599812,0,-0.061000001,-0.061000001,30.34760094},
		  {0.0042,-0.0066,4.902599812,0,-0.061000001,-0.061000001,30.34760094},
		  {0.0036,-0.0084,4.893599987,0,-0.061000001,0,30.35350037},
		  {0.0036,-0.0084,4.893599987,0,-0.061000001,0,30.35350037},
		  {-0.0036,-0.0054,4.901400089,-0.061000001,-0.061000001,-0.061000001,30.353500366},
		  {-0.0036,-0.0054,4.901400089,-0.061000001,-0.061000001,-0.061000001,30.3535003},
		  {-0.0018,-0.003,4.910399914,-0.061000001,-0.061000001,0,30.33589935},
		  {-0.0018,-0.003,4.910399914,-0.061000001,-0.061000001,0,30.33589935},
		  {0.485599995,-0.0341,5.011000156,0,0,0,33.71239853},
		  {0.485599995,-0.0341,5.011000156,0,0,0,33.71239853},
		  {0.485599995,-0.0341,5.011000156,0,0,0,33.71239853},
		  {0.488599986,-0.037700001,5.015200138,-0.061000001,0,0,33.68880081},
		  {0.488599986,-0.037700001,5.015200138,-0.061000001,0,0,33.68880081},
		  {0.488599986,-0.037700001,5.015200138,-0.061000001,0,0,33.68880081},
		  {0.488599986,-0.037700001,5.015200138,-0.061000001,0,0,33.68880081},
		  {0.488599986,-0.0359,5.015799999,0,0,-0.061000001,33.68289948},
		  {0.488599986,-0.0359,5.015799999,0,0,-0.061000001,33.68289948},
		  {0.488599986,-0.0359,5.015799999,0,0,-0.061000001,33.68289948},
		  {0.488599986,-0.0359,5.015799999,0,0,-0.061000001,33.68289948},
		  {0.489199996,-0.0383,5.01639986,0,0,0,33.71820068},
		  {0.489199996,-0.0383,5.01639986,0,0,0,33.71820068},
		  {0.489800006,-0.037099998,5.021800041,0,-0.061000001,0,33.71239853},
		  {0.489800006,-0.037099998,5.021800041,0,-0.061000001,0,33.71239853},
		  {0.489800006,-0.037099998,5.021800041,0,-0.061000001,0,33.71239853},
		  {0.488599986,-0.0359,5.01940012,0,0,0,33.69469833},
		  {0.488599986,-0.0359,5.01940012,0,0,0,33.69469833},
		  {0.488599986,-0.0359,5.01940012,0,0,0,33.69469833},
		  {0.488599986,-0.0359,5.01940012,0,0,0,33.69469833},
		  {0.490399987,-0.036499999,5.02120018,0,0,0,33.69469833}};
   */

  /*char inputArray[] = "0.054000001,6,0.0024,-0.0006,3.856600046,-0.061000001,-0.061000001,0,34.83589935\n"
		  "0.066,7,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n"
		  "0.07,8,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n"
		  "0.082999997,9,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n"
		  "0.085000001,10,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n"
		  "0.101999998,11,-0.0006,-0.0048,4.633200169,-0.061000001,-0.061000001,0,34.81240082\n"
		  "0.109999999,12,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n"
		  "0.112999998,13,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n"
		  "0.131999999,14,0.003,-0.003,4.794199944,0,-0.061000001,0,34.83000183\n"
		  "0.140000001,15,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n"
		  "0.143999994,16,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n"
		  "0.156000003,17,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n"
		  "0.164000005,18,0,-0.006,4.858300209,-0.061000001,-0.061000001,0,34.81240082\n"
		  "0.172999993,19,-0.003,-0.0054,4.869699955,-0.061000001,-0.061000001,0,34.81240082\n"
		  "0.174999997,20,-0.003,-0.0054,4.869699955,-0.061000001,-0.061000001,0,34.81240082\n"
		  "0.187999994,21,-0.0042,-0.0054,4.879799843,0,-0.061000001,-0.061000001,34.84180069\n"
		  "0.192000002,22,-0.0042,-0.0054,4.879799843,0,-0.061000001,-0.061000001,34.84180069\n"
		  "0.211999997,23,-0.0054,-0.006,4.889999866,0,-0.061000001,0,34.81240082\n"
		  "0.221000001,24,-0.0072,-0.0042,4.89659977,-0.061000001,-0.061000001,0,34.83000183\n"
		  "0.224999994,25,-0.0072,-0.0042,4.89659977,-0.061000001,-0.061000001,0,34.83000183\n"
		  "0.241999999,26,-0.006,-0.0048,4.901400089,-0.061000001,-0.061000001,0,34.83000183\n"
		  "0.250999987,27,-0.003,-0.0012,4.902599812,-0.061000001,-0.061000001,-0.061000001,34.82410049\n"
		  "0.254999995,28,-0.003,-0.0012,4.902599812,-0.061000001,-0.061000001,-0.061000001,34.82410049\n"
		  "0.268000007,29,-0.0036,-0.0036,4.90320015,-0.061000001,-0.061000001,0,34.83589935\n"
		  "0.275999993,30,-0.003,0,4.899600029,0,-0.061000001,0,34.83000183\n"
		  "0.280000001,31,-0.003,0,4.899600029,0,-0.061000001,0,34.83000183\n"
		  "0.301999986,32,-0.003,-0.0006,4.900800228,0,-0.061000001,0,34.83000183\n"
		  "0.31099999,33,-0.0006,-0.0006,4.901400089,-0.061000001,-0.061000001,-0.061000001,34.84180069\n"
		  "0.31400001,34,-0.0006,-0.0006,4.901400089,-0.061000001,-0.061000001,-0.061000001,34.84180069\n"
		  "0.326000005,35,0,-0.0006,4.901400089,-0.061000001,-0.061000001,0,34.85350037\n"
		  "0.330000013,36,0,-0.0006,4.901400089,-0.061000001,-0.061000001,0,34.85350037";
	*/

  char inputArray[] = "0.054000001,6,0.0024,-0.0006,3.856600046,-0.061000001,-0.061000001,0,34.83589935\n";

  //This displays the header which explains the formating of the data outputed.
  uint8_t header2[81];
  sprintf(header2, "\r\nAccel X (g),Accel Y (g),Accel Z (g),Gyro X (dps),Gyro Y (dps),Gyro Z (dps),Temp");
  HAL_UART_Transmit(&huart2, header2, sizeof(header2), 1000);


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
	  encrypt(inputArray);
	   uint8_t msg[200];
	  sprintf(msg, "%s",encryptedData);
	  HAL_UART_Transmit(&huart2, encryptedData, sizeof(encryptedData), 1000);

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
unsigned long long int ENCmodpow(int base, int power, int mod)
{
        int i;
        unsigned long long int result = 1;
        for (i = 0; i < power; i++)
        {
                result = (result * base) % mod;
        }
        return result;
}

void encrypt(char msg[]) {
    int m;
    unsigned long long int c;

	int i;
	int elements = sizeof(&msg);
	unsigned long long int temp[elements];
    for (i = 0; msg[i]!= '}'; i++)
    {
        c = ENCmodpow(msg[i],e,n);

        // FORMATS the encrypted data into a string.
        if(encryptedBits==0){
            //sprintf(encryptedData, "%s",(char)c);
            encryptedData[i] = c;
        }else{
            //sprintf(encryptedData, "%s\n%llu",encryptedData,(char)c);
            encryptedData[i] = c +"\n";
        }
        encryptedBits++;
    }

    //Call compression
    //encode();

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
