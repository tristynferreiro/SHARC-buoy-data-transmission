# SHARC Buoy Data Transmission
This project deals with implementation of encryption and compression of data from sensors on an STM32F0-discovery. This project aims to provide a solution to the data size problems highlighted in the SHARC Buoy design by [Jamie Nicholas Jacobson](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Docs/Other/thesis_ebe_2021_jacobson%20jamie%20nicholas.pdf). Additionally, encryption is added to further secure the data being transmitted. This repo details a solution designed, tested, documented and implemented by **[Shameera Cassim](https://github.com/ShameeraC) and [Tristyn Ferreiro](https://github.com/tristynferreiro)**.

Although the system is designed as a solution for the SHARC Buoy system, it is also an encryption-compression solution for other resource-constrained systems. If used for other systems, the algorithm should be optimised for the expected data. This involves testing different compression window and encryption key sizes.
  
  
![Screenshot](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Docs/Other/system_interfacing.png)

The [final design report](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Docs/Reports/Final%20Report/Final_Report.pdf) provides a holistic and detailed description of the sytem's interactions, tests and designs.

### What you will need
- STM32F0Discovery board
- SparkFun 9Dof Breakout board's ICM20948
- A computer with STM32CubeIDE installed on it
- FTDI chip with micro or USB connector
- Jumper cables
- 2x microUSB to USB connector
