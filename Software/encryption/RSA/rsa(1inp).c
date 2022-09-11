// modified from code available at https://github.com/yigitusta/RSA-Implementation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#define MAX_VALUE 65535

#define E_VALUE 3 /*65535*/

uint16_t e = E_VALUE, p, q;
uint32_t n, phi, d;
char ch[];

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

//char* readInput(char* inp[]) {
//    int count = 0;
//    ehil
//    if(ch[count]==EOF)
//		{
//			break;
//		}
//		else
//		{
//            ch[count]=fgetc(infile);
//            count++;
//		}
//}

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
int m, n, e;
        unsigned long long int c;

        FILE *inp = fopen("public.txt", "r");
        fscanf(inp, "%d %d", &n, &e);
        fclose(inp);

	printf("ciphertext c = ");

	int i;
	FILE *outp = fopen("ciphertext.txt", "w");

	for (i = 0; msg[0][i]!= '\0'; i++)
	{
        	c = ENCmodpow(msg[0][i],e,n);
		printf("%llu ", c);
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
        FILE *inp;

        inp = fopen("private.txt", "r");
        fscanf(inp, "%d %d", &n, &d);
        fclose(inp);

        inp = fopen("pq.txt", "r");
        fscanf(inp, "%d %d", &p, &q);
        fclose(inp);

	printf("message: ");
	inp = fopen("ciphertext.txt", "r");

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
    int enc;
    int dec;
    char *s;
    char* c[] = {"iAmAFile", "hello"};

//    if (argc != 4) {
//        printf("Usage: RSA e/d infile outfile\n\te = encode\td = decode\n");
//        return 1;
//    }
    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D' || *s == 'e' || *s == 'E')) {
        enc = (*s == 'e' || *s == 'E');
        dec = (*s == 'd' || *s == 'D');
    }
//    else {
//        printf("? %s\n", s);  return 1;
//    }
//    if ((infile  = fopen(argv[2], "rb")) == NULL) {
//        printf("? %s\n", argv[2]);  return 1;
//    }
//    if ((outfile = fopen(argv[3], "a")) == NULL) {
//        printf("? %s\n", argv[3]);  return 1;
//    }
    rsa_init();
    if (enc) {encrypt(c);}
    else if(dec) {decrypt();}
    fclose(infile);  fclose(outfile);
    return 0;
}

