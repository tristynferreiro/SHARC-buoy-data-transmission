# Encryption-Compression
This folder contains all previous developed versions of the the combined algorithm code needed for testing. These versions were created throughout the development of the final solution and serve a different functions.

## SHARC_buoy_transmission_data
The STM32CubeIDE project of the combined algorithms (without sensor interfacing - a hard coded array data is compressed and encrypted instead). Can be flashed onto a STM32F051 compatible microcontroller.

**Note:** When increasing the amount of data to compress and encrypt, the compression and encryption array sizes also need to be increased. Not increasing the array sizes will result in the program crashing or not running correctly.

## combined_chars.c
This version of the program:
- prints the compressed-encrypted data to a file as **characters**.
- uses a hardcoded array of values as input.
- uses a **static** encryption key.

### Running:
#### Unix:
$ gcc combined_chars.c <br />
$ ./.aout e *nameOfOutfile*.
#### Windows:
$ gcc combined_chars.c <br />
$ a.exe e *nameOfOutfile*

## combined_dynamic_key.c
This version of the program:
- prints the compressed-encrypted data to a file as **characters**.
- uses a hardcoded array of values as input.
- uses a **dynamic** encryption key.

### Running:
#### Unix:
$ gcc combined_dynamic_key.c <br />
$ ./.aout e *nameOfOutfile*.
#### Windows:
$ gcc combined_dynamic_key.c <br />
$ a.exe e *nameOfOutfile*


## combined_integers.c
This version of the program:
- prints the compressed-encrypted data to a file as **integers**.
- uses a hardcoded array of values as input.
- uses a **static** encryption key.

### Running:
#### Unix:
$ gcc combined_integers.c <br />
$ ./.aout e *nameOfOutfile*.
#### Windows:
$ gcc combined_integers.c <br />
$ a.exe e *nameOfOutfile*
