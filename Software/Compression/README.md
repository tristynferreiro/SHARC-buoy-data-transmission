# Compression
This folder contains all previous developed versions of the the lzss compression algorithms required for development of the subsystem. The steps for compiling and running these algorithms are outlined below. These algorithms are adapted from 

## lzss_decompression.c
If the data being decompressed is the output of an stm32f0 project run the clean.py first:
$ python3 clean.py

Then copy the output into the source code. The comma seperated list is the *inputComp[]* array.
The last integer value is the *compDataArraySize*.

Finally:
$ gcc lzss_decompression.c <br />
$ ./a.out d outputFileName



## lzss_modified_stm32_test.c

## lzss_modified_array_input.c


## lzss.c
This is the original unmodified version of the lzss algorithm (Haruhiko Okumura's LZSS encoder-decoder) that is used in this system design. This version of the program:
- uses a file as input
- compresses the input file to a char-valued file output

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc lzss.c
$ ./a.out e *\<input file name\>* *\<compressed file name\>*
$ ./a.out d *\<compressed file name\>* *\<output file name\>*
```
**_Note: e=encode (compress) and d=decode (decompress)_**

## Other/
Contains previous and other versions of the modified algorithm that were used throughout the system design:
- lzss_modified_array_input_char.c and lzss_modified_file_input_char.c : function the same as the other versions but use char data types instead. These versions did not work when implementing the algorithms in

