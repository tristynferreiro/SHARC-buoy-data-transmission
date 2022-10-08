# Software
## SHARC_buoy/
This is the latest version of the complete working project. It reads accelerometer and gyroscope data from the icm20948, encrypts and compresses it and then transmits the compressed-encrypted data over UART. 

This project can be opened using STM32CubesIDE and then flashed onto an STM32F0 compatible board. Make sure to following the sensor setup instructions as described [here](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Software/Sensor/README.md). 

<u>Note:</u> the project specifically uses SPI, however the IMU is I2C compatible.

## combined/
This contains all previous developed versions of the the combine algorithm code needed for testing. These were created throughout the development of the final solution and serve a different function.

### SHARC_buoy_transmission_data
The STM32CubeIDE project of the combined algorithms (without sensor interfacing, instead hard coded array data is compressed and encrypted). Can be flashed on to a STM32F051 compatible microcontroller.

### combined_chars.c
This version of the program:
- prints the compressed-encrypted data to a file as **characters**.
- uses a hardcoded array of values as input.
- uses a **static** encryption key.

#### running:
$ gcc combined.c
$ ./.aout e *nameOfOutfile*.

### combined_dynamic_key.c
This version of the program:
- prints the compressed-encrypted data to a file as **characters**.
- uses a hardcoded array of values as input.
- uses a **dynamic** encryption key.

#### running:
$ gcc combined_dynamic_key.c
$ ./.aout e *nameOfOutfile*.

### combined_integers.c
This version of the program:
- prints the compressed-encrypted data to a file as **integers**.
- uses a hardcoded array of values as input.
- uses a **static** encryption key.

#### running:
$ gcc combined_integers.c
$ ./.aout e *nameOfOutfile*.