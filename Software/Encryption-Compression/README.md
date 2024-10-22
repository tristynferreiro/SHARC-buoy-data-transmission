# Encryption-Compression
This folder contains all previous developed versions of the the combined algorithm code needed for testing. These versions were created throughout the development of the final solution and serve different functions.
<br /><br />
**_Note: to run any of the commands using a Windows-based terminal, replace ./a.out with a.exe_** <br />
**_Additionally, all the programs are only for encoding data, the [decompression](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Software/Compression/lzss_decompression.c) and [decryption](https://github.com/tristynferreiro/SHARC_buoy_data_transmission/blob/main/Software/Encryption/RSA/rsa_decryption.c) algorithms can be used to recover the original data_**

## SHARC_buoy_no_sensor/
The STM32CubeIDE project of the combined algorithms (without sensor interfacing - a hard coded array data is compressed and encrypted instead). Can be flashed onto a STM32F051 compatible microcontroller.

**Note:** When increasing the amount of data to compress and encrypt, the compression and encryption array sizes also need to be increased. Not increasing the array sizes will result in the program crashing or not running correctly.

## combined_chars.c
This version of the program:
- prints the compressed-encrypted data to a file as **characters**.
- uses a hardcoded array of values as input.
- uses a **static** encryption key.

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc combined_chars.c
$ ./a.out e <output file name>
```

## combined_dynamic_key.c
This version of the program:
- prints the compressed-encrypted data to a file as **characters**.
- uses a hardcoded array of values as input.
- uses a **dynamic** encryption key.

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc combined_dynamic_key.c
$ ./a.out e <output file name>
```

## combined_integers.c
This version of the program:
- prints the compressed-encrypted data to a file as **integers**.
- uses a hardcoded array of values as input.
- uses a **static** encryption key.

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc combined_integers.c
$ ./a.out e <output file name>
```
