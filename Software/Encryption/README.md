# Encryption #
This contains both ChaCha20Poly1305 and RSA code. ChaCha20Poly1305 was the original algorithm chosen in the paper design, however the system was updated to utilize RSA for encryption in the first progress report. **RSA is used in the final working system. ** <br /><br /> 

## RSA/
This contains all previous developed versions of the the RSA implementation needed for testing. These were created throughout the development of the final solution and serve a different function. The RSA implementation is based on that developed by Yiğit Usta available at https://github.com/yigitusta/RSA-Implementation. <br /><br />

The different versions are:
(note: for all the below c files, the input file is not used for encryption but as their main methods were used largely for PC testing, they were not edited to accomodate for encryption and compression calls separately) <br /><br /> 

**Note: to run any of the below commands on a Unix based system, replace a.exe with ./a.out**

### - rsa #
This version of the code dynamically generates a key each time it is run. It takes in a hard-coded array and outputs a file for encryption and takes in and outputs a file for decryption. <br /><br />

To run this program a Windows terminal and using gcc compiling: <br />
\$ gcc rsa.c <br /><br />

encryption: <br />
\$ a.exe rsa e \<input file\> \<output file\> <br />

decryption: <br />
\$ a.exe rsa d \<input file\> \<output file\> <br /><br />

### - rsa_modified_array_output #
This version of the code is the same as rsa above except that the **encryption function writes the encrypted data to an array** instead of a file. <br /><br />

To run this program a Windows terminal and using gcc compiling: <br />
\$ gcc rsa_modified_array_output.c <br /><br />

encryption: <br />
\$ a.exe rsa_modified_array_output e \<input file\> \<output file\> <br />

decryption: <br />
\$ a.exe rsa_modified_array_output d \<input file\> \<output file\> <br /><br />

### - rsa_modified_array_output_fixed_key #
This version of the code is the same as rsa_modified_array_output above except that a **hard-coded key is used.** <br /><br />

To run this program a Windows terminal and using gcc compiling: <br />
\$ gcc rsa_modified_array_output_fixed_key.c <br /><br />

encryption: <br />
\$ a.exe rsa_modified_array_output_fixed_key e \<input file\> \<output file\>

decryption: <br />
\$ a.exe rsa_modified_array_output_fixed_key d \<input file\> \<output file\> <br /><br />

### - rsa_decrypted #
This version of the code includes a hard-coded key and is only used for decryption. It takes in a takes in an encrypted file and outputs a decrypted file. <br /><br />

To run this program a Windows terminal and using gcc compiling: <br />
\$ gcc rsa_decrypted.c <br />
\$ a.exe rsa_decrypted d \<input file\> \<output file\> <br /><br />


## ChaCha20Poly1305V2/
This contains the orignal attempt at implementing encryption using ChaCha20Poly1305. This implementation is based on that developed by Jonas Schnelli available at https://github.com/jonasschnelli/chacha20poly1305. **This code is not used in the final version of the project.** It was kept in the git repository for completeness.

