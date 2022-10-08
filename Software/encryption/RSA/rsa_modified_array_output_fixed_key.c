// modified from code available at https://github.com/yigitusta/RSA-Implementation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#define MAX_VALUE 8

#define E_VALUE 3 /*65535*/

int e = E_VALUE;
int encryptedData[20000];

int n = 187;
int d = 107;

int p = 11;
int q = 17;

FILE *infile, *outfile;

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
        encryptedData[i] = c;
    }
}

void readFromFile(FILE* file, char* arr[]) {
    int wordCount = 0, i;
    //Actions
    if(file != NULL) {
    while(!feof(file)) {
        fscanf(file, "%c", &arr[wordCount]);
        wordCount++;
    }
}
}
void encrypt(FILE* file) {
    char* arr[90000];
    readFromFile(file, arr);
    //encrypt2(arr);
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
        int h, m, qInv, m1m2;
        unsigned long long int c, dP, dQ, m1, m2;
        FILE *inp, *out;

	while (fscanf(infile, "%llu", &c) != EOF)
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
	fclose(infile);
	fclose(outfile);

}

int main(int argc, char *argv[])
{
    int enc;
    int dec;
    char *s;
    char c[] = {"0.0017,0.0002,-0.0051,0.2439,-0.1829,-0.1220}"};

    //    if (argc != 4) {
    //     printf("Usage: RSA e/d infile outfile\n\te = encode\td = decode\n");
    //      return 1;
    //    }

    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D' || *s == 'e' || *s == 'E')) {
        enc = (*s == 'e' || *s == 'E');
        dec = (*s == 'd' || *s == 'D');
    }
   else {
       printf("? %s\n", s);  return 1;
   }
    if ((infile  = fopen(argv[2], "rb")) == NULL) {
       printf("? %s\n", argv[2]);  return 1;
    }
    if ((outfile = fopen(argv[3], "w")) == NULL) {
        printf("? %s\n", argv[3]);  return 1;
    }
    if (enc) {encrypt2(c);}
    else if(dec) {decrypt();}
    fclose(infile);  fclose(outfile);
    return 0;
}
