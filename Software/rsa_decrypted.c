// modified from code available at https://github.com/yigitusta/RSA-Implementation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#define MAX_VALUE 16

#define E_VALUE 3 /*65535*/

int e = 3;
int encryptedData[20000];

int n = 187;
int d = 107;

int p = 11;
int q = 17;

FILE *infile, *outfile;

int DECmodpow(int base, int power, int mod)
{
        int i;
        int result = 1;
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
        int c, dP, dQ, m1, m2;
        FILE *inp, *out;

	while (fscanf(infile, "%d", &c) != EOF)
	{
        	dP = d % (p - 1);
        	dQ = d % (q - 1);
        	qInv = inverse(q,p);
        	m1 = DECmodpow(c,dP,p);
        	m2 = DECmodpow(c,dQ,q);
        	m1m2 = m1 - m2;
        	if (m1m2 < 0) {
                	m1m2 += p;
        	}
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

    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D' )) {
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
    if(dec) {decrypt();}
    fclose(infile);  fclose(outfile);
    return 0;
}

