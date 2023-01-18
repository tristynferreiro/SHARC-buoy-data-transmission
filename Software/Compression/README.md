# Compression
This folder contains all previous developed versions of the the lzss compression algorithms required for development of the subsystem. The steps for compiling and running these algorithms are outlined below.

## lzss_decompression.c
If the data being decompressed is the output of an stm32f0 project run the clean.py first:
$ python3 clean.py

Then copy the output into the source code. The comma seperated list is the *inputComp[]* array.
The last integer value is the *compDataArraySize*.

Finally:
$ gcc lzss_decompression.c <br />
$ ./a.out d outputFileName

## lzss.c
On a linux terminal and using gcc compiling:
$ gcc lzss.c
For compression:
$ ./a.out e inputFileName compressedFileName

For decompression:
$ ./a.out d compressedFileName outputFileName

## lzss_modified_array_input.c

## lzss_modified_file_input.c

## lzss_modified_stm32_test.c

