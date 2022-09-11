/*
 * Authors: Tristyn Ferreiro and Shameera Cassim
 * This is program takes in a file and compresses/decompresses or encrypts/decrypts it to a new file.
 *
 * The compression/decompression is LZSS encoder-decoder (Haruhiko Okumura; public domain) and the
 * encryption/decryption is ...
 */

#include <stdio.h>
#include <stdlib.h>

#define EI 11  /* typically 10..13 */ /** increasing this, increases the compression time but decreases compressed file size*/
#define EJ  4  /* typically 4..5 */
#define P   1  /* If match length <= P then output one character */
#define N (1 << EI)  /* buffer size */
#define F ((1 << EJ) + 1)  /* lookahead buffer size */

int bit_buffer = 0, bit_mask = 128;
unsigned long codecount = 0, textcount = 0;
unsigned char buffer[N * 2];
FILE *infile, *outfile;

void error(void)
{
    printf("Output error\n");  exit(1);
}

void putbit1(void)
{
    bit_buffer |= bit_mask;
    if ((bit_mask >>= 1) == 0) {
        if (fputc(bit_buffer, outfile) == EOF) error();
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void putbit0(void)
{
    if ((bit_mask >>= 1) == 0) {
        if (fputc(bit_buffer, outfile) == EOF) error();
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void flush_bit_buffer(void)
{
    if (bit_mask != 128) {
        if (fputc(bit_buffer, outfile) == EOF) error();
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

/* compresses the data */
void encode(void)
{
    int i, j, f1, x, y, r, s, bufferend, c;

    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    for (i = N - F; i < N * 2; i++) {
        if ((c = fgetc(infile)) == EOF) break;
        buffer[i] = c;  textcount++;
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
                if ((c = fgetc(infile)) == EOF) break;
                buffer[bufferend++] = c;  textcount++;
            }
        }
    }
    flush_bit_buffer();
    printf("text:  %ld bytes\n", textcount);
    printf("code:  %ld bytes (%ld%%)\n",
        codecount, (codecount * 100) / textcount);
}

int getbit(int n) /* get n bits */
{
    int i, x;
    static int buf, mask = 0;

    x = 0;
    for (i = 0; i < n; i++) {
        if (mask == 0) {
            if ((buf = fgetc(infile)) == EOF) return EOF;
            mask = 128;
        }
        x <<= 1;
        if (buf & mask) x++;
        mask >>= 1;
    }
    return x;
}

/* decompresses lzss encoded file */
void decode(void)
{
    int i, j, k, r, c;

    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
    while ((c = getbit(1)) != EOF) {
        if (c) {
            if ((c = getbit(8)) == EOF) break;
            fputc(c, outfile);
            buffer[r++] = c;  r &= (N - 1);
        } else {
            if ((i = getbit(EI)) == EOF) break;
            if ((j = getbit(EJ)) == EOF) break;
            for (k = 0; k <= j + 1; k++) {
                c = buffer[(i + k) & (N - 1)];
                fputc(c, outfile);
                buffer[r++] = c;  r &= (N - 1);
            }
        }
    }
}

// modified from code available at https://github.com/yigitusta/RSA-Implementation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#define MAX_VALUE 65535

#define E_VALUE 3 /*65535*/

uint16_t e = E_VALUE, p, q;
uint32_t n, phi, d;

FILE *infile, *outfile;
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
}

/*int chooseE(int phi)
{
	srand(time(NULL));
	int e = rand() % 65533 + 3;
	while (gcd(e,phi) != 1 || e < 4)
	{
		srand(time(NULL));
		e = rand() % 65533 + 3;
	}
	return e;
}*/

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

void encrypt(char* msg[]) {
    rsa_init();
    int m, n, e;
    unsigned long long int c;

    FILE *inp = fopen("public.txt", "r");
    fscanf(inp, "%d %d", &n, &e);
    fclose(inp);

	int i;
	int elements = sizeof(&msg);
	unsigned long long int temp[elements];
	printf("%d \n", elements);
	FILE *outp = fopen("ciphertext.csv", "w");
    printf("ciphertext = ");
        for (i = 0; msg[0][i]!= '\n'; i++)
        {
            c = ENCmodpow(msg[0][i],e,n);
            printf("%llu \n", c);
            fprintf(outp, "%llu\n", c);

        }
    printf("\n");
    fclose(outp);

}

unsigned long long int DECmodpow(unsigned long long int base, int power, int mod)
{
        int i;
        unsigned long long int result = 1;
        for (i = 0; i < power; i++)
        {
                result = (result * base) % mod;
        }
        return result;
}

int inverse(int a, int mod)     /*find inverse of number a in % mod*/
{                               /*inverse of a = i*/
        int aprev, iprev, i = 1, atemp, itemp;

        aprev = mod, iprev = mod;
        while (a != 1)
        {
                atemp = a;
                itemp = i;
                a = aprev - aprev / atemp * a;
                i = iprev - aprev / atemp * i;
                aprev = atemp;
                iprev = itemp;
                while (i < 0)
                        i += mod;
        }

        return i;
}


void decrypt() {
        int d, n, p, q, h, m, qInv, m1m2;
        unsigned long long int c, dP, dQ, m1, m2;
        FILE *inp, *out;

        inp = fopen("private.txt", "r");
        fscanf(inp, "%d %d", &n, &d);
        fclose(inp);

        inp = fopen("pq.txt", "r");
        fscanf(inp, "%d %d", &p, &q);
        fclose(inp);

	inp = fopen("ciphertext.csv", "r");

	    printf("message: ");

	while (fscanf(inp, "%llu", &c) != EOF)
	{
        	dP = d % (p - 1);
        	dQ = d % (q - 1);
        	qInv = inverse(q,p);
        	m1 = DECmodpow(c,dP,p);
        	m2 = DECmodpow(c,dQ,q);
        	m1m2 = m1 - m2;
        	if (m1m2 < 0)
                	m1m2 += p;
       		h = (qInv * m1m2) % p;
        	m = m2 + h * q;
        	fprintf(outfile, "%c", m);
            printf("%c", m);
	}
	fclose(inp);
	printf("\n");
        /*printf("dP: %u\n", dP);
        printf("dQ: %u\n", dQ);
        printf("qInv: %u\n", qInv);
        printf("m1: %u\n", m1);
        printf("m2: %u\n", m2);
        printf("h: %u\n", h);*/

}

int main(int argc, char *argv[])
{
    int task;
    int task2;
    int task3;
    char *s;

    if (argc != 4) {
        printf("Usage: lzss comp/decomp infile outfile\n\te = encode\td = decode\n");
        return 1;
    }
    s = argv[1];
    char* arr= {"0.054000001","6,0.0024","-0.0006","3.856600046","-0.061000001","-0.061000001","0,34.83589935", "0.066","7","0.0048"}
    if (s[1] == 0 && (*s == 'decomp' || *s == 'Decomp' || *s == 'comp' || *s == 'Comp' || *s =='E' || *s == 'e' || *s == 'd' || *s == 'D')) {
        task = (*s == 'c' || *s == 'C');
        task2 = (*s == 'e' || *s == 'E');
        task3 = (*s == 'd' || *s == 'D');
    }
    else {
        printf("? %s\n", s);  return 1;
    }
    if ((infile  = fopen(argv[2], "rb")) == NULL) {
        printf("? %s\n", argv[2]);  return 1;
    }
    if ((outfile = fopen(argv[3], "wb")) == NULL) {
        printf("? %s\n", argv[3]);  return 1;
    }


    if (task) encode(); else if(task2) encrypt(arr); else if (task3) decrypt(); else decode();


    fclose(infile);  fclose(outfile);
    return 0;
}
