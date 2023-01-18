# ICM20948
This README aims to explain interfacing the STM32F051-discovery board and the [SparkFun 9DoF Breakout board](https://www.sparkfun.com/products/15335) using SPI. This project has been adapted from [this gitHub repo](https://github.com/mokhwasomssi/stm32_hal_icm20948) for compatibilit with the stm32f0. Only the icm20948.h file and some methods from icm20948.c were copied across and changed for use in the new project.

### Step 1: The board and SPI
The STM32F051-discovery board has two SPI connection groups.

Following the current setup of the project, the follwing connections should be made:

![Screenshot](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Docs/Other/icm20948_interfacing.png)

_Pin configurations can be changed in the ICM20948.ioc file in STM32CubeIDE._

### Step 2: Flashing the board
The sensor's software needs to be loaded onto the Discovery board. The ICM20948 STM32CubeIDE project is flashed onto the discovery board by connecting the discovery board to the computer and running the project in the STM32CubeIDE. 