/**
**************************************************
Info:		encryption (RSA) and compresstion (lzss)
Author:		Tristyn Ferreiro and Shameera Cassim
****************************************************
This code compresses and encrypts data in a hard coded array and prints the result 
to a file (as CHARACTERS). The encryption uses a DYNAMIC KEY.

The compression algorithm uses a modified version of (Haruhiko Okumura; public domain)'s 
lzss encoder. Encryption is based off of AES encryption. Modifications to both of these 
algorithms have been made to suite the desing requirements of this project.
******************************************************************************
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

int bit_buffer = 0, bit_mask = 128;
unsigned long codecount = 0, textcount = 0;
unsigned char buffer[N * 2];

//For encryption:
#define MAX_VALUE 65535
#define E_VALUE 3 /*65535*/

uint16_t e = E_VALUE, p, q;
uint32_t n, phi, d;

FILE *outfile;
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
 * This method has been added to store the compression bits in an array that will be passed to the encryption algorithm.
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

void compress(void) // should be modified to take in value
{
    //printf("enc Data = %s\n", encryptedData);
    
    int i, j, f1, x, y, r, s, bufferend, c;
    
    int counter = 0;
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    for (i = N - F; i < N * 2; i++) {
        if (counter > strlen(encryptedData)) break;
        c = encryptedData[counter];
        buffer[i] = c;  counter++;
        //printf("c = %d\n", c);;
        //textcount++;
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
                //textcount++;
            }
        }
    }
    
    //WRITE COMPRESSED DATA to FILE
    for (int jk=0;jk<compressedBits-1;jk++){
        fputc(compressed[jk],outfile);
    }
    //fprintf(f, "%s",compressed);
}

uint32_t findD(uint16_t e, uint32_t phi)
{
	uint32_t eprev, dprev, d = 1, etemp, dtemp;

	eprev = phi, dprev = phi;
	while (e != 1)
	{
		etemp = e;
		dtemp = d;
		e = eprev - eprev / etemp * e;
		d = dprev - eprev / etemp * d;
		eprev = etemp;
		dprev = dtemp;
		while (d < 0)
			d += phi;
	}

	return d;
}

int ifprime(uint16_t n)
{
	uint16_t i;
	for (i = 2; i <= n / 2; i++)
	{
		if (n % i == 0)
			return 0;
	}
	return 1;
}

uint16_t gcd(uint16_t num1, uint32_t num2)
{
	uint16_t i, temp;
	if (num1 > num2)
	{
		temp = num1;
		num1 = num2;
		num2 = temp;
	}
	for (i = num1; i > 0; i--)
	{
		if (num1 % i == 0 && num2 % i == 0)
			return i;
	}
    return -1;
}

uint16_t getprime()
{
	uint16_t n;
	do
	{
		srand(time(NULL));
		n = rand() % MAX_VALUE + 5;
	}while  (!ifprime(n));
	return n;
}

void setprimes(uint16_t e, uint16_t *p, uint16_t *q, uint32_t *n, uint32_t *phi)
{
	do
	{
		*p = getprime();
		do
			*q = getprime();
		while(*p == *q);

		*n = *p * *q;
		*phi = *n - *p - *q + 1;
	}while (gcd(e,*phi) != 1);
}

void rsa_init()
{
	setprimes(e, &p, &q, &n, &phi);

	d = findD(e,phi);

	FILE *outp = fopen("public.txt", "w");
	fprintf(outp, "%"PRIu32" %"PRIu16, n, e);
	fclose(outp);

	outp = fopen("private.txt", "w");
	fprintf(outp, "%"PRIu32" %"PRIu32, n, d);
	fclose(outp);

	outp = fopen("pq.txt", "w");
    fprintf(outp, "%"PRIu16" %"PRIu16, p, q);
    fclose(outp);
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
    //rsa_init();
    int m, n, e;
    unsigned long long int c;
    //unsigned char c;

    FILE *inp = fopen("public.txt", "r");
    fscanf(inp, "%d %d", &n, &e);
    fclose(inp);

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
    compress();

}

int main(int argc, char *argv[])
{
    int enc;
    int dec;
    char *s;
    //char* c[3] = {"˜"};
    //char c[] = 
    char input[] = "0.054000001,6,0.0024,-0.0006,3.856600046,-0.061000001,-0.061000001,0,34.83589935\n0.066,7,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n0.07,8,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n0.082999997,9,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n0.085000001,10,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n0.101999998,11,-0.0006,-0.0048,4.633200169,-0.061000001,-0.061000001,0,34.81240082\n0.109999999,12,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n0.112999998,13,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n0.131999999,14,0.003,-0.003,4.794199944,0,-0.061000001,0,34.83000183\n0.140000001,15,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.143999994,16,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.156000003,17,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.164000005,18,0,-0.006,4.858300209,-0.061000001,-0.061000001,0,34.81240082\n0.172999993,19,-0.003,-0.0054,4.869699955,-0.061000001,-0.061000001,0,34.81240082}";
    
    if (argc != 3) {
        printf("Usage: combined e/d outfile\n\te = encrypt and compress\n");
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
    fclose(outfile);
    return 0;
}