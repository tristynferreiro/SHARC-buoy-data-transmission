# Software
This project deals with implementation of encryption and compression of data from sensors on an STM32F0-discovery. This project aims to provide a solution to the data size problems highlighted in the SHARC Buoy design by [Jamie Nicholas Jacobson](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Docs/thesis_ebe_2021_jacobson%20jamie%20nicholas.pdf). This repo details a solution designed, tested, documented and implemented by Shameeras Cassima and Tristyn Ferreiro.

## SHARC_buoy/
[final design report]()
This is the latest version of the complete working project. It reads accelerometer and gyroscope data from the icm20948, encrypts and compresses it and then transmits the compressed-encrypted data over UART. 

This project can be opened using STM32CubesIDE and then flashed onto an STM32F0 compatible board. Make sure to following the sensor setup instructions as described [here](). 

<u>Note:</u> the project specifically uses SPI, however the IMU is I2C compatible.

### Datasheets/
Contains the data sheets of the components used in this project.

### Reports/
Contains all the reports created throughout the design process of this project. This includes the paper design, two progress reports and the final design report. 

The  provides a holistic and detailed description of the sytem's interactions, tests and designs.

## Software/
Contains all the source code used in this project. For both the compression and encryption algorithms, there are multiple veersions of the source code which served different purposes during the design and implementation phases of the project. They are named descriptively.

Both the compression and encryption folders contain READMEs explaining how to compile and run the files.

## Testing/
Contains all the data recorded throughout the design of the system.
