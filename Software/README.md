# Software
## SHARC_buoy/
This is the latest version of the complete working project. It reads accelerometer and gyroscope data from the icm20948, encrypts and compresses it and then transmits the compressed-encrypted data over UART. 

This project can be opened using STM32CubesIDE and then flashed onto an STM32F0 compatible board. Make sure to following the sensor setup instructions as described [here](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Software/Sensor/README.md). 

<u>Note:</u> the project specifically uses SPI, however the IMU is I2C compatible.

**Important:** When increasing the data to compress and encrypt, the compression and encryption array sizes also need to be increased. Otherwise thr program will crash/not run correctly.

## combined/
This contains all previous developed versions of the the combined algorithm code needed for testing. These versions were created throughout the development of the final solution and serve a different function.

The diffrent versions are:
- SHARC_buoy_transmission_data
- combined_chars.c
- combined_dynamic_key.c
- combined_integers.c

## compression/
This folder contains all previous developed versions of the the compression algorithm needed for testing. These versions were created throughout the development of the final solution and serve different functions.<br /><br />

A version lzss compression was used in the solution of this project. 

## encryption/
This folder contains all previous developed versions of the the encryption algorithm needed for testing. These versions were created throughout the development of the final solution and serve different functions. <br /><br />

RSA encryption was used in the solution of this project. 

## sensor/
This is the contains the stm32CubeIDE project used to interface with the SparkFun 9Dof Breakout board's ICM20948. The project is based on [this gitHub repo](https://github.com/mokhwasomssi/stm32_hal_icm20948) but some functions and interfaces were changed for use in this project. The project is also setup for use on an stm32f4 so adaptations needed to be made. Only the [icm20948.h](https://github.com/mokhwasomssi/stm32_hal_icm20948/blob/master/icm20948.h) file and some methods from [icm20948.c](https://github.com/mokhwasomssi/stm32_hal_icm20948/blob/master/icm20948.h) were copied across and changed for use in the new project. 

# Common Bug fixes
### I changed the stm32 projects' input data and the program no longer runs
This is probably because the size of the compression and ecryption arrays are too small. Increase the sizes, reflash the microcontroller and test. Do this until the program runs again.
However, there will be a point where the STM's RAM limit is reached, in this case decrease the size of the input data.
