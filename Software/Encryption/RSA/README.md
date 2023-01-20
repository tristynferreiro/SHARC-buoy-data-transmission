# RSA Encryption
Please note: While the below C files take in an input file as an argument, these are not used for encryption. The main methods of these files were used largely for PC testing and were not edited to accomodate for encryption and decryption calls separately. 
<br /><br />
**_Note: to run any of the above commands using a Windows-based terminal, replace ./a.out with a.exe_**

## rsa.c
This version of the code dynamically generates a key each time it is run. It takes in a hard-coded array and outputs a file for encryption and takes in and outputs a file for decryption.

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc rsa.c 
```
**Encryption:** 
```bash
$ ./a.out rsa e <input file name> <output file name> 
```
**Decryption:** <br />
```bash
$ ./a.out rsa d <input file name> <output file name> 
```

## rsa_modified_array_output.c
This version of the code is the same as rsa above except that the **encryption function writes the encrypted data to an array** instead of a file.

### To run this program on a Unix-based terminal using gcc compiling:
```bash
gcc rsa_modified_array_output.c
```
**Encryption:** <br />
```bash
$ ./a.out rsa_modified_array_output e <input file name> <output file name> 
```
**Decryption:** <br />
```bash
$ ./a.out rsa_modified_array_output d <input file name> <output file name> 
```

## rsa_modified_array_output_fixed_key.c
This version of the code is the same as rsa_modified_array_output above except that a **hard-coded key is used.**

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc rsa_modified_array_output_fixed_key.c
```
**Encryption:** <br />
```bash
$ ./a.out rsa_modified_array_output_fixed_key e <input file name> <output file name> 
```
**Decryption:** <br />
```bash
$ ./a.out rsa_modified_array_output_fixed_key d <input file name> <output file name> 
```

## rsa_decrypted.c
This version of the code includes a hard-coded key and is only used for decryption. It takes in a takes in an encrypted file and outputs a decrypted file.

### To run this program on a Unix-based terminal using gcc compiling:
```bash
$ gcc rsa_decrypted.c
$ ./a.out rsa_decrypted d <input file name> <output file name> 
```

