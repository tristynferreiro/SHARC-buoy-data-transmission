# Software
## SHARC_buoy/
This is the latest version of the complete working project. It reads accelerometer and gyroscope data from the icm20948, encrypts and compresses it and then transmits the compressed-encrypted data over UART.

In its current form, the project will take a fixed number of sensor data recordings (1 second apart) and store them in an array. This block of data is then sent off to encryption and compression and then trasmitted. Then there is a 5 second delay before the process is restarted. If you want to remove the restart or increase the delay consult lines 252-255 of the main.c file.

#### running the project
This project can be opened using STM32CubesIDE and then flashed onto an STM32F0 compatible board. Make sure to follow the sensor setup instructions as described [here](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Software/Sensor/README.md). 
<br/>
*The project specifically uses SPI, however the IMU is I2C compatible.*
<br/><br/>
**Important:** When increasing the input data, the input data, compression and encryption array sizes also need to be increased. Otherwise thr program will crash/not run correctly.

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
This folder contains all previous developed versions of the the encryption algorithm needed for testing. These versions were created throughout the development of the final solution and serve different functions. <br />
RSA encryption was used in the solution of this project. 

## sensor/
This contains the stm32CubeIDE project used to interface with the SparkFun 9Dof Breakout board's ICM20948. The project is based on [this gitHub repo](https://github.com/mokhwasomssi/stm32_hal_icm20948) but some functions and interfaces were changed for use in this project. That repo project is also setup for use on an stm32f4 so adaptations needed to be made for use in this project. Only the [icm20948.h](https://github.com/mokhwasomssi/stm32_hal_icm20948/blob/master/icm20948.h) file and some methods from [icm20948.c](https://github.com/mokhwasomssi/stm32_hal_icm20948/blob/master/icm20948.h) were copied across and changed for use in the new project. 

## Scripts/
Contains all scripts used for gathering the data trasmitted from the stm32f0.
-serialReceive.py
-clean.py
-clean_multiTransmits.py

# Common Bug fixes
### I changed the stm32 projects' input data and the program no longer runs
When changing the number of recordings to read, a series of updates to the arrays in the code need to be made:
1. If formatting changes are made:
- update the size of reading[] accordingly.
- update the strncat() call to the same size as the new reading[] size.
- update the the [clean.py]() script to reflect any changes if necessary 
    (only needed if terminating/ start sequence characters are changed)
2. Update the numReadings variable to the desired value
3. Update the inputArray[] size using the formula in the code comments
4. Update size of encryptionData[] array
5. Update size of compressed[] array
6. Reflash the microcontroller and test. Steps 4 and 5 might need to be repeated to find the right value. (Idea: make their values very big and then reduce the size until it breaks).

However, there will be a point where the STM's RAM limit is reached, in this case decrease the size of the input data.
