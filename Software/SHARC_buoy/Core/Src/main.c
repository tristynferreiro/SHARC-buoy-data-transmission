/* USER CODE BEGIN Header */
/**
  *************************************
Info:		SHARC buoy compression and encryption
Author:		Tristyn Ferreiro and Shameera Cassim
*************************************
This code compresses and encrypts data on the stm32.

Code is also provided to send data from the STM32 to other devices using HAL's
UART protocol. You will need a serial port reader on your PC to receive the data.

The output data is compressed-encrypted data.

The compression algorithm uses a modified version of (Haruhiko Okumura; public domain)'s
lzss encoder.
Encryption is based off of AES encryption.
The icm20948.h and other methods are adapted from mokhwasomssi's github

In future versions, the data will be read from the sensor HAT ICM2098 chip.

NOTE: When increasing the input data, the input data, compression and encryption
      array sizes also need to be increased. Otherwise the program will crash/not
      run correctly.
******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "icm20948.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* FOR COMPRESSION */
#define EI  6  // typically 10..13 - due to the limited ability of the STM, 6 is used
#define EJ  5  // typically 4..5
#define P   1  //If match length <= P then output one character
#define N (1 << EI)  // buffer size
#define F ((1 << EJ) + 1)  // lookahead buffer size

/* FOR ENCRYPTION */
//these variables are used when a dynamic key is implemented for encryption
//#define MAX_VALUE 16 // size of key
//#define E_VALUE 3 /*65535*/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// IMU VARIABLES
axises my_gyro;
axises my_accel;
axises my_mag;

static float gyro_scale_factor;
static float accel_scale_factor;

// COMPRESSION VARIABLES
int numRecordings =0; // this keeps track of the number of recordings.

int bit_buffer = 0, bit_mask = 128;
int buffer[N * 2];
int compressed[300]; // size of data to compress at one time (should be at least the size of encryption array)
int compressedBits =0; //keep track of compressed bits for transmission

// ENCRYPTION VARIABLES
int e = 3;
int n = 187;
int d = 107;
int p = 11;
int q = 17;
int encryptedData[300]; // passed to compression
int encryptedBits = 0; // needed for use in compression

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void icm20948_init();
void icm20948_gyro_read(axises* data);
void icm20948_accel_read(axises* data);
void icm20948_gyro_read_dps(axises* data);
void icm20948_accel_read_g(axises* data);
/* Sub Functions */
void icm20948_accel_full_scale_select(accel_full_scale full_scale);
void icm20948_gyro_full_scale_select(gyro_full_scale full_scale);
void icm20948_accel_calibration();
void icm20948_gyro_calibration();
void icm20948_accel_sample_rate_divider(uint16_t divider);
void icm20948_gyro_sample_rate_divider(uint8_t divider);
void icm20948_accel_low_pass_filter(uint8_t config);
void icm20948_gyro_low_pass_filter(uint8_t config);
void icm20948_odr_align_enable();
void icm20948_clock_source(uint8_t source);
void icm20948_spi_slave_enable();
void icm20948_sleep();
void icm20948_wakeup();
void icm20948_device_reset();
bool icm20948_who_am_i();
/*Static functions*/
static void cs_high();
static void cs_low();
static void select_user_bank(userbank ub);
static uint8_t read_single_icm20948_reg(userbank ub, uint8_t reg);
static void write_single_icm20948_reg(userbank ub, uint8_t reg, uint8_t val);
static uint8_t* read_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t len);
static void write_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t* val, uint8_t len);

/* compression and encryption */
int correctBitbuffer(int bitbuffer);
void store(int bitbuffer);
void putbit1(void);
void flush_bit_buffer(void);
void output1(int c);
void output2(int x, int y);
void encode(int encryptedData[], int encryptedBits);
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
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  icm20948_init();
 // ak09916_init();

  //This displays the header which explains the formating of the data outputted.
  uint8_t header[77];
  sprintf((char*)header, "\r\nAccel X (g),Accel Y (g),Accel Z (g),Gyro X (dps),Gyro Y (dps),Gyro Z (dps)");
  HAL_UART_Transmit(&huart2, header, sizeof(header), 1000);

  int numReadings = 3; // number of sensor readings you want to take
  int numDataRecordings =0;
  int run = 0; // whether or not to run encrypt&compress

  char inputArray[112] =""; // size = numReadings * 37+1
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// raw data
	icm20948_accel_read(&my_accel);
	icm20948_gyro_read(&my_gyro);

	// or unit conversion
	icm20948_gyro_read_dps(&my_gyro);
	icm20948_accel_read_g(&my_accel);

	if(numDataRecordings <=numReadings-1){
		/*
		 * If any changes are made to the formatting of the readings, inputArray[] size needs to be updated.
		 * The reading array size must also be updated. The strncat() must also be updated.
		 */
		char reading[37];
		sprintf(reading, "\r\n%.2f,%.2f,%.2f,%.2f,%.2f,%.2f;",my_accel.x,my_accel.y,my_accel.z,
				my_gyro.x,my_gyro.y,my_gyro.z);
		// HAL_UART_Transmit(&huart2, (uint8_t*)reading, sizeof(reading), 1000);

		strncat(inputArray,reading,37); //if reading formatting is changed this length needs to be updated

		if(numDataRecordings ==numReadings-1){
			strcat(inputArray,"}");
			HAL_UART_Transmit(&huart2, (uint8_t*)inputArray, sizeof(inputArray), 1000);
			HAL_Delay(1000);
			run++;
		}
		numDataRecordings++;
	}

	/* Run compression and encryption */
	if (run ==1) {
		char start[4];
		sprintf(start, "\r\n#");
		HAL_UART_Transmit(&huart2, (uint8_t*)start, sizeof(start), 1000);

		encrypt(inputArray);
		int count = 0;
		while (count < compressedBits) {
			char temp [7];
			/*
			 * note that the numbers are of different lengths and so this causes extra blank space in the formatting
			 * which the transmission will fill with random characters. Make sure to used clean.py or adapt it to remove
			 * the unwanted characters.
			 */
			sprintf(temp, "\r\n%d,",compressed[count]);
			HAL_UART_Transmit(&huart2, (uint8_t*)temp, sizeof(temp), 1000);
			count++;
		}
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
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : SPI2_CS_Pin */
  GPIO_InitStruct.Pin = SPI2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI2_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/********************************
 * THIS IS THE IMU SENSOR CODE
 *******************************/
void icm20948_init()
{
	while(!icm20948_who_am_i());

	icm20948_device_reset();
	icm20948_wakeup();

	icm20948_clock_source(1);
	icm20948_odr_align_enable();

	icm20948_spi_slave_enable();

	icm20948_gyro_low_pass_filter(0);
	icm20948_accel_low_pass_filter(0);

	icm20948_gyro_sample_rate_divider(0);
	icm20948_accel_sample_rate_divider(0);

	icm20948_gyro_calibration();
	icm20948_accel_calibration();

	icm20948_gyro_full_scale_select(_2000dps);
	icm20948_accel_full_scale_select(_2g);
}

void icm20948_gyro_read(axises* data)
{
	uint8_t tempH = read_single_icm20948_reg(ub_0, B0_GYRO_XOUT_H);
	uint8_t tempL = read_single_icm20948_reg(ub_0, B0_GYRO_XOUT_L);

	data->x = (int16_t)(tempH<< 8|tempL);

	tempH = read_single_icm20948_reg(ub_0, B0_GYRO_YOUT_H);
	tempL = read_single_icm20948_reg(ub_0, B0_GYRO_YOUT_L);
	data->y = (int16_t)(tempH<< 8|tempL);

	tempH = read_single_icm20948_reg(ub_0, B0_GYRO_ZOUT_H);
	tempL = read_single_icm20948_reg(ub_0, B0_GYRO_ZOUT_L);
	data->z = (int16_t)(tempH<< 8|tempL);
}

void icm20948_accel_read(axises* data)
{
	uint8_t tempH = read_single_icm20948_reg(ub_0, B0_ACCEL_XOUT_H);
	uint8_t tempL = read_single_icm20948_reg(ub_0, B0_ACCEL_XOUT_L);

	data->x = (int16_t)(tempH<< 8|tempL);

	tempH = read_single_icm20948_reg(ub_0, B0_ACCEL_YOUT_H);
	tempL = read_single_icm20948_reg(ub_0, B0_ACCEL_YOUT_L);
	data->y = (int16_t)(tempH<< 8|tempL);

	tempH = read_single_icm20948_reg(ub_0, B0_ACCEL_ZOUT_H);
	tempL = read_single_icm20948_reg(ub_0, B0_ACCEL_ZOUT_L);
	data->z = (int16_t)(tempH<< 8|tempL);
	// Add scale factor because calibraiton function offset gravity acceleration.
}

void icm20948_gyro_read_dps(axises* data)
{
	icm20948_gyro_read(data);

	data->x /= gyro_scale_factor;
	data->y /= gyro_scale_factor;
	data->z /= gyro_scale_factor;
}

void icm20948_accel_read_g(axises* data)
{
	icm20948_accel_read(data);

	data->x /= accel_scale_factor;
	data->y /= accel_scale_factor;
	data->z /= accel_scale_factor;
}

bool icm20948_who_am_i()
{
	uint8_t icm20948_id = read_single_icm20948_reg(ub_0, B0_WHO_AM_I);

	if(icm20948_id == ICM20948_ID)
		return true;
	else
		return false;
}

void icm20948_device_reset()
{
	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, 0x80 | 0x41);
	HAL_Delay(100);
}

void icm20948_wakeup()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_PWR_MGMT_1);
	new_val &= 0xBF;

	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, new_val);
	HAL_Delay(100);
}

void icm20948_sleep()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_PWR_MGMT_1);
	new_val |= 0x40;

	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, new_val);
	HAL_Delay(100);
}

void icm20948_spi_slave_enable()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_USER_CTRL);
	new_val |= 0x10;

	write_single_icm20948_reg(ub_0, B0_USER_CTRL, new_val);
}

void icm20948_clock_source(uint8_t source)
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_PWR_MGMT_1);
	new_val |= source;

	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, new_val);
}

void icm20948_odr_align_enable()
{
	write_single_icm20948_reg(ub_2, B2_ODR_ALIGN_EN, 0x01);
}

void icm20948_gyro_low_pass_filter(uint8_t config)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1);
	new_val |= config << 3;

	write_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1, new_val);
}

void icm20948_accel_low_pass_filter(uint8_t config)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_ACCEL_CONFIG);
	new_val |= config << 3;

	write_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1, new_val);
}

void icm20948_gyro_sample_rate_divider(uint8_t divider)
{
	write_single_icm20948_reg(ub_2, B2_GYRO_SMPLRT_DIV, divider);
}

void icm20948_accel_sample_rate_divider(uint16_t divider)
{
	uint8_t divider_1 = (uint8_t)(divider >> 8);
	uint8_t divider_2 = (uint8_t)(0x0F & divider);

	write_single_icm20948_reg(ub_2, B2_ACCEL_SMPLRT_DIV_1, divider_1);
	write_single_icm20948_reg(ub_2, B2_ACCEL_SMPLRT_DIV_2, divider_2);
}

void icm20948_gyro_calibration()
{
	axises temp;
	int32_t gyro_bias[3] = {0};
	uint8_t gyro_offset[6] = {0};

	for(int i = 0; i < 100; i++)
	{
		icm20948_gyro_read(&temp);
		gyro_bias[0] += temp.x;
		gyro_bias[1] += temp.y;
		gyro_bias[2] += temp.z;
	}

	gyro_bias[0] /= 100;
	gyro_bias[1] /= 100;
	gyro_bias[2] /= 100;

	// Construct the gyro biases for push to the hardware gyro bias registers,
	// which are reset to zero upon device startup.
	// Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format.
	// Biases are additive, so change sign on calculated average gyro biases
	gyro_offset[0] = (-gyro_bias[0] / 4  >> 8) & 0xFF;
	gyro_offset[1] = (-gyro_bias[0] / 4)       & 0xFF;
	gyro_offset[2] = (-gyro_bias[1] / 4  >> 8) & 0xFF;
	gyro_offset[3] = (-gyro_bias[1] / 4)       & 0xFF;
	gyro_offset[4] = (-gyro_bias[2] / 4  >> 8) & 0xFF;
	gyro_offset[5] = (-gyro_bias[2] / 4)       & 0xFF;

	write_multiple_icm20948_reg(ub_2, B2_XG_OFFS_USRH, gyro_offset, 6);
}

void icm20948_accel_calibration()
{
	axises temp;
	uint8_t* temp2;
	uint8_t* temp3;
	uint8_t* temp4;

	int32_t accel_bias[3] = {0};
	int32_t accel_bias_reg[3] = {0};
	uint8_t accel_offset[6] = {0};

	for(int i = 0; i < 100; i++)
	{
		icm20948_accel_read(&temp);
		accel_bias[0] += temp.x;
		accel_bias[1] += temp.y;
		accel_bias[2] += temp.z;
	}

	accel_bias[0] /= 100;
	accel_bias[1] /= 100;
	accel_bias[2] /= 100;

	uint8_t mask_bit[3] = {0, 0, 0};

	temp2 = read_multiple_icm20948_reg(ub_1, B1_XA_OFFS_H, 2);
	accel_bias_reg[0] = (int32_t)(temp2[0] << 8 | temp2[1]);
	mask_bit[0] = temp2[1] & 0x01;

	temp3 = read_multiple_icm20948_reg(ub_1, B1_YA_OFFS_H, 2);
	accel_bias_reg[1] = (int32_t)(temp3[0] << 8 | temp3[1]);
	mask_bit[1] = temp3[1] & 0x01;

	temp4 = read_multiple_icm20948_reg(ub_1, B1_ZA_OFFS_H, 2);
	accel_bias_reg[2] = (int32_t)(temp4[0] << 8 | temp4[1]);
	mask_bit[2] = temp4[1] & 0x01;

	accel_bias_reg[0] -= (accel_bias[0] / 8);
	accel_bias_reg[1] -= (accel_bias[1] / 8);
	accel_bias_reg[2] -= (accel_bias[2] / 8);

	accel_offset[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  	accel_offset[1] = (accel_bias_reg[0])      & 0xFE;
	accel_offset[1] = accel_offset[1] | mask_bit[0];

	accel_offset[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  	accel_offset[3] = (accel_bias_reg[1])      & 0xFE;
	accel_offset[3] = accel_offset[3] | mask_bit[1];

	accel_offset[4] = (accel_bias_reg[2] >> 8) & 0xFF;
	accel_offset[5] = (accel_bias_reg[2])      & 0xFE;
	accel_offset[5] = accel_offset[5] | mask_bit[2];

	write_multiple_icm20948_reg(ub_1, B1_XA_OFFS_H, &accel_offset[0], 2);
	write_multiple_icm20948_reg(ub_1, B1_YA_OFFS_H, &accel_offset[2], 2);
	write_multiple_icm20948_reg(ub_1, B1_ZA_OFFS_H, &accel_offset[4], 2);
}

void icm20948_gyro_full_scale_select(gyro_full_scale full_scale)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1);

	switch(full_scale)
	{
		case _250dps :
			new_val |= 0x00;
			gyro_scale_factor = 131.0;
			break;
		case _500dps :
			new_val |= 0x02;
			gyro_scale_factor = 65.5;
			break;
		case _1000dps :
			new_val |= 0x04;
			gyro_scale_factor = 32.8;
			break;
		case _2000dps :
			new_val |= 0x06;
			gyro_scale_factor = 16.4;
			break;
	}

	write_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1, new_val);
}

void icm20948_accel_full_scale_select(accel_full_scale full_scale)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_ACCEL_CONFIG);

	switch(full_scale)
	{
		case _2g :
			new_val |= 0x00;
			accel_scale_factor = 16384;
			break;
		case _4g :
			new_val |= 0x02;
			accel_scale_factor = 8192;
			break;
		case _8g :
			new_val |= 0x04;
			accel_scale_factor = 4096;
			break;
		case _16g :
			new_val |= 0x06;
			accel_scale_factor = 2048;
			break;
	}

	write_single_icm20948_reg(ub_2, B2_ACCEL_CONFIG, new_val);
}

/* Static Functions */

static void cs_high()
{
	HAL_GPIO_WritePin(ICM20948_SPI_CS_PIN_PORT, ICM20948_SPI_CS_PIN_NUMBER, SET);
}

static void cs_low()
{
	HAL_GPIO_WritePin(ICM20948_SPI_CS_PIN_PORT, ICM20948_SPI_CS_PIN_NUMBER, RESET);
}

static void select_user_bank(userbank ub)
{
	uint8_t write_reg[2];
	write_reg[0] = WRITE | REG_BANK_SEL;
	write_reg[1] = ub;

	cs_low();
	HAL_SPI_Transmit(ICM20948_SPI, write_reg, 2, 10);
	cs_high();
}

static uint8_t read_single_icm20948_reg(userbank ub, uint8_t reg)
{
	uint8_t read_reg = READ | reg;
	uint8_t reg_val;
	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(ICM20948_SPI, &read_reg, 1, 1000);
	HAL_SPI_Receive(ICM20948_SPI, &reg_val, 1, 1000);
	cs_high();

	return reg_val;
}

static void write_single_icm20948_reg(userbank ub, uint8_t reg, uint8_t val)
{
	uint8_t write_reg[2];
	write_reg[0] = WRITE | reg;
	write_reg[1] = val;

	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(ICM20948_SPI, write_reg, 2, 1000);
	cs_high();
}

static uint8_t* read_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t len)
{
	uint8_t read_reg = READ | reg;
	static uint8_t reg_val[6];
	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(ICM20948_SPI, &read_reg, 1, 1000);
	HAL_SPI_Receive(ICM20948_SPI, reg_val, len, 1000);
	cs_high();

	return reg_val;
}

static void write_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t* val, uint8_t len)
{
	uint8_t write_reg = WRITE | reg;
	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(ICM20948_SPI, &write_reg, 1, 1000);
	HAL_SPI_Transmit(ICM20948_SPI, val, len, 1000);
	cs_high();
}
/********************************
 * THIS IS THE COMPRESSION CODE
 *******************************/
int correctBitbuffer(int bitbuffer) {
	int val;
	int tempvar = (int)log2(bitbuffer)+1;
	if (tempvar >=8) {
		val = 256 - bitbuffer;
		val = -1 * val;
		return val;
	}
	return bitbuffer;
}
/**
 * This method has been added to store the compression encoded bits in one array for printing/transmission.
 */
void store(int bitbuffer){
	compressed[compressedBits] = correctBitbuffer(bitbuffer);
    compressedBits++;
}

void putbit1(void)
{
    bit_buffer |= bit_mask;
    if ((bit_mask >>= 1) == 0) {
        store(bit_buffer);
        bit_buffer = 0;  bit_mask = 128;
    }
}

void putbit0(void)
{
    if ((bit_mask >>= 1) == 0) {
        store(bit_buffer);
        bit_buffer = 0;
        bit_mask = 128;
    }
}

void flush_bit_buffer(void)
{
    if (bit_mask != 128) {
        store(bit_buffer);
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

void encode(int encryptedData[], int encryptedBits)
{
    int i, j, f1, x, y, r, s, bufferend, c;
    int counter = 0;

    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    for (i = N - F; i < N * 2; i++) {
        if (counter > encryptedBits) break;
        c = encryptedData[counter];
        buffer[i] = c;  counter++;
        //printf("HERE2: %d\n",buffer[i]);
        //printf("c = %d\n", c);;
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
                if (counter > encryptedBits) break;
                c = encryptedData[counter];
                buffer[bufferend++] = c;  counter++;
            }
        }
    }
    /*
    // Can be used to check that compression is working at this point
    int count = 0;
    while (count < compressedBits) {
		char temp[4];
		sprintf(temp, "%i, ",compressed[count]);
		HAL_UART_Transmit(&huart2, temp, sizeof(temp), 1000);
		count++;
    } */

}



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
           /*
           //used for error checking
           if (i > 0) {
                sprintf(mesg, "%d and i-1 =%dP",encryptedData[i], encryptedData[i-1]);
                HAL_UART_Transmit(&huart2, mesg, sizeof(mesg), 1000);
            }*/
        }
        //call compression
        encode(encryptedData, encryptedBits);
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
