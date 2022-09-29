/*
 * This program takes in a file, encrypts it using RSA encryption passes it to the compression algorithm which compresses it using lzss compression and then
 * prints the result to a file.
 *
 * This uses fixed key encryption.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

//For compression:
#define EI 11  /* typically 10..13 */
#define EJ  5  /* typically 4..5 */
#define P   1  /* If match length <= P then output one character */
#define N (1 << EI)  /* buffer size */
#define F ((1 << EJ) + 1)  /* lookahead buffer size */

//For encryption:
#define MAX_VALUE 16
#define E_VALUE 3 /*65535*/

//For compression:
int bit_buffer = 0, bit_mask = 128;
unsigned long codecount = 0, textcount = 0;
unsigned char buffer[N * 2];
//uint64_t buffer[N * 2];

//For encryption:
int e = E_VALUE;

int n = 187;
int d = 107;

int p = 11;
int q = 17;

FILE *infile, *outfile;
/**
 * This array stores the encoded bit_buffers of all the data. The size needs to be chosen based on the number of bits of data.
 * For the STM32F0 implementation, the size will need to be determine based on available space on the STM. This will likely be
 * through trial and error
 */
char compressed[2000]; // needs to be atleast the size of the input data (minimum). this size should be the limit of data stored at any one time
int compressedBits =0;

/*
 * This is the mock input array of data to be compressed
 */

//char encryptedData[][2000];
char encryptedData[20000];
int encryptedBits = 0;
void error(void)
{
    //printf("Output error\n");
    exit(1);
}

/**
 * This method has been added to store the compression encoded bits in an array that will be passed to the encryption algorithm.
 */
void store(int bitbuffer){
    compressed[compressedBits]=bitbuffer;
    compressedBits++;
}

void putbit1(void)
{
    bit_buffer |= bit_mask;
    if ((bit_mask >>= 1) == 0) {
        store(bit_buffer);
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void putbit0(void)
{
    if ((bit_mask >>= 1) == 0) {
        store(bit_buffer);
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void flush_bit_buffer(void)
{
    if (bit_mask != 128) {
        store(bit_buffer);
        codecount++;
    }
}

void output1(int c)
{
    int mask;

    putbit1();
    mask = 256;
    while (mask >>= 1) {
        if (c & mask) putbit1();
        else putbit0();
    }
}

void output2(int x, int y)
{
    int mask;

    putbit0();
    mask = N;
    while (mask >>= 1) {
        if (x & mask) putbit1();
        else putbit0();
    }
    mask = (1 << EJ);
    while (mask >>= 1) {
        if (y & mask) putbit1();
        else putbit0();
    }
}

void encode(void)
{

    int i, j, f1, x, y, r, s, bufferend, c;

    int counter = 0;
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    for (i = N - F; i < N * 2; i++) {
        if (counter > strlen(encryptedData)) break;
        c = encryptedData[counter];
        buffer[i] = c;  counter++;
        //printf("c = %d\n", c);;
    }
    bufferend = i;  r = N - F;  s = 0;
    while (r < bufferend) {
        f1 = (F <= bufferend - r) ? F : bufferend - r;
        x = 0;  y = 1;  c = buffer[r];
        for (i = r - 1; i >= s; i--)
            if (buffer[i] == c) {
                for (j = 1; j < f1; j++)
                    if (buffer[i + j] != buffer[r + j]) break;
                if (j > y) {
                    x = i;  y = j;
                }
            }
        if (y <= P) {  y = 1;  output1(c);  }
        else output2(x & (N - 1), y - 2);
        r += y;  s += y;
        if (r >= N * 2 - F) {
            for (i = 0; i < N; i++) buffer[i] = buffer[i + N];
            bufferend -= N;  r -= N;  s -= N;
            while (bufferend < N * 2) {
                if (counter > strlen(encryptedData)) break;
                c = encryptedData[counter];
                buffer[bufferend++] = c;  counter++;
            }
        }
    }

    //WRITE COMPRESSED DATA to FILE
    for (int jk=0;jk<compressedBits-1;jk++){
        fputc(compressed[jk],outfile);
    }
    //fprintf(f, "%s",compressed);
}

unsigned long long int ENCmodpow(int base, int power, int mod)
{
        int i;
        unsigned long long int result = 1;
        for (i = 0; i < power; i++)
        {
                result = (result * base) % mod;
        }
        return result;
}

void encrypt2(char msg[]) {
    int m;
    unsigned long long int c;

	int i;
	int elements = sizeof(&msg);
	unsigned long long int temp[elements];
    for (i = 0; msg[i]!= '}'; i++)
    {
        c = ENCmodpow(msg[i],e,n);

        // FORMATS the encrypted data into a string.
        if(encryptedBits==0){
            sprintf(encryptedData, "%llu",c);
        }else{
            sprintf(encryptedData, "%s\n%llu",encryptedData,c);
        }
        encryptedBits++;
    }

    //Call compression
    encode();

}

int main(int argc, char *argv[])
{
    int enc;
    int dec;
    char *s;

    char input[] = "13, 14, 15, 16}";
    if (argc != 3) {
        printf("Usage: combined e outfile\n\te = encrypt and compress\n");
        return 1;
    }
    s = argv[1];

    if (s[1] == 0 && (*s == 'e' || *s == 'E')) {
        enc = (*s == 'e' || *s == 'E');
    }
   else {
       printf("? %s\n", s);  return 1;
   }
    if ((outfile = fopen(argv[2], "w")) == NULL) {
        printf("? %s\n", argv[2]);  return 1;
    }
    if (enc) {encrypt2(input);}
    fclose(infile);  fclose(outfile);
    return 0;
}
