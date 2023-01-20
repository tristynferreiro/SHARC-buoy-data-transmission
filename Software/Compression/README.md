# Compression
This folder contains all previous developed versions of the the lzss compression algorithms required for development of the subsystem. The steps for compiling and running these algorithms are outlined below. These algorithms are adapted from Haruhiko Okumura's LZSS encoder-decoder.
<br /><br />
**Note: to run any of the commands using a Windows-based terminal, replace ./a.out with a.exe**

## lzss_decompression.c
This is an adaptation of the original decoder. This version of the program:
- takes a lzss-compressed file as input
- decompresses to a file
- does not include the compression algorithm

### To run this program on a Unix-based terminal using gcc compiling:
If the data being decompressed is the output of an stm32f0 project, run the clean.py first:
```bash
$ python3 clean.py
```
Then delete the last value in the output file and use that as the input file for decompression. Otherwise just use the compressed file as the input file:

```bash
$ gcc lzss_decompression.c 
$ ./a.out d <input file name> <output file name>
```

## lzss_decompression_array_input.c
This is an adaptation of the original decoder. This version of the program:
- uses an integer array as input
- decompresses to a file
- does not include the compression algorithm

### To run this program on a Unix-based terminal using gcc compiling:
If the data being decompressed is the output of an stm32f0 project, run the clean.py first:
```bash
$ python3 clean.py
```
The values in the output file must be copied into the decompression source code before compilation. The comma-seperated list must be copied into the _inputComp[]_ array and the last integer value is the _compDataArraySize_:

```c
    int inputComp[]={-100,50,-2};
    int compDataArraySize = 70;
```
_Note: if the output is not from an stm32f0 project, the array and array size need to be populated manually with the values in the lzss-compressed file.

Then
```bash
$ gcc lzss_decompression.c 
$ ./a.out d <output file name>
```
## lzss_modified_array_input.c
This version of the program:
- uses an integer array as input
- compresses to file output (in integers)
- does not include the decompression algorithm

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc lzss_modified_array_input.c
$ ./a.out e <compressed file name>
```

## lzss.c
This is the original unmodified version of the lzss algorithm (Haruhiko Okumura's LZSS encoder-decoder) that is used in this system design. This version of the program:
- uses a file as input
- compresses to file output (in chars)

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc lzss.c
$ ./a.out e <input file name> <compressed file name>
$ ./a.out d <compressed file name> <output file name>
```
_Note: e=encode (compress) and d=decode (decompress)_

## Other/
Contains previous and other versions of the modified algorithm that were used throughout the system design:
- lzss_modified_array_input_char.c and lzss_modified_file_input_char.c : function the same as the other versions but use char data types instead. These versions did not work when implementing the algorithms on the stm32f0


