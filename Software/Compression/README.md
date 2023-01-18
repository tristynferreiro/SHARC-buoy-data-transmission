# Compiling the source code

## lzss_decompression.c
If the data being decompressed is the output of an stm32f0 project run the clean.py first:
$ python3 clean.py

Then copy the output into the source code. The comma seperated list is the *inputComp[]* array.
The last integer value is the *compDataArraySize*.

Finally:
$ gcc lzss_decompression.c
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

