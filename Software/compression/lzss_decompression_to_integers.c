/* 
 * This is a modified version of the original LZSS decoder (Haruhiko Okumura; public domain) 
 * Instead of taking in a file an array of values is used as input.
 * The decompressed data is then printed to a file.
 */

#include <stdio.h>
#include <stdlib.h>

#define EI 11  /* typically 10..13 */
#define EJ  5  /* typically 4..5 */
#define P   1  /* If match length <= P then output one character */
#define N (1 << EI)  /* buffer size */
#define F ((1 << EJ) + 1)  /* lookahead buffer size */

int bit_buffer = 0, bit_mask = 128;
int buffer[N * 2];

FILE *outfile;

/*
int inputComp[]={152,75,166,19,89,164,193,249,5,49,150,77,165,143,190,25,132,202,105,44,150,191,104,166,15,214,9,156,186,113,53,155,77,159,220,51,71,250,3,250,10,109,49,126,66,128,66,230,15,246,9,163,254,130,103,53,156,78,103,32,80,8};*/
int inputComp[]={-128,-46,123,117,88,12,105,-8,67,-120};
int lineNumber =0;
int compDataArraySize = 10;

int getbit(int n) /* get n bits */
{
    int i, x;
    static int buf, mask = 0;
    
    x = 0;
    for (i = 0; i < n; i++) {
        if (mask == 0) {
            if (lineNumber>=compDataArraySize) break;
            
            printf("%d; %d\n",lineNumber,inputComp[lineNumber]);
            
            buf = inputComp[lineNumber];
            mask = 128;
            lineNumber++;
        }
        x <<= 1;
        if (buf & mask) x++;
        mask >>= 1;
    }
    return x;
}

void decode(void)
{
    int i, j, k, r, c;
    
    lineNumber=0;
    
   //printf("SIZE: %d",compDataArraySize);
    
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
    while ((c = getbit(1)) != EOF) {
        if (c) {
            if (lineNumber >= compDataArraySize) break;
            c=getbit(8);
            printf("+%d\n",c);
            fprintf(outfile, "%d\n",c);
            //fputc(c, outfile);
            buffer[r++] = c;  r &= (N - 1);
        } else {
            i = getbit(EI);
            j = getbit(EJ);
            if (lineNumber>=compDataArraySize) break;
            for (k = 0; k <= j + 1; k++) {
                c = buffer[(i + k) & (N - 1)];
                printf("=%d\n",c);
                fprintf(outfile, "%ld\n",c);
                //fputc(c, outfile);
                buffer[r++] = c;  r &= (N - 1);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int denc;
    char *s;
    
    if (argc != 3) {
        printf("Usage: lzss d outfile\n\td = decode\n");
        return 1;
    }
    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D'))
        denc = (*s == 'd' || *s == 'D');
    else {
        printf("? %s\n", s);  return 1;
    }
    if ((outfile = fopen(argv[2], "wb")) == NULL) {
        printf("? %s\n", argv[2]);  return 1;
    }
    if (denc) decode();
    fclose(outfile);
    return 0;
}