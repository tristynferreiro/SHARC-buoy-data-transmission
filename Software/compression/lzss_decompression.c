/*
 * This is a modified version of the original LZSS decoder (Haruhiko Okumura; public domain)
 * Instead of taking in a file an array of values is used as input.
 * The decompressed data is then printed to a file.
 */

#include <stdio.h>
#include <stdlib.h>

#define EI  6  /* typically 10..13 */
#define EJ  5  /* typically 4..5 */
#define P   1  /* If match length <= P then output one character */
#define N (1 << EI)  /* buffer size */
#define F ((1 << EJ) + 1)  /* lookahead buffer size */

int bit_buffer = 0, bit_mask = 128;
int buffer[N * 2];

FILE *infile, *outfile;

int compDataArraySize = 70;
int lineNumber =0;

int getbit(int n) /* get n bits */
{
    int i, x;
    static int buf, mask = 0;

    x = 0;
    for (i = 0; i < n; i++) {
        if (mask == 0) {
            if ((fscanf(infile, "%d", &buf)) == EOF) return EOF;
            //printf("=%d\n",buf);
            mask = 128;
            lineNumber++;
        }
        x <<= 1;
        if (buf & mask) x++;
        mask >>= 1;
    }
    return x;
}

void decompress()
{
    int i, j, k, r, c;

    lineNumber=0;
    
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
    while ((c = getbit(1)) != EOF) {
        if (c) {
            if ((c = getbit(8)) == EOF) break;
            //printf("+%d\n",c);
            fprintf(outfile, "%d\n",c);
            //fputc(c, outfile);
            buffer[r++] = c;  r &= (N - 1);
        } else {
            if ((i = getbit(EI)) == EOF) break;
            if ((j = getbit(EJ)) == EOF) break;
            for (k = 0; k <= j + 1; k++) {
                c = buffer[(i + k) & (N - 1)];
                //printf("=%d\n",c);
                fprintf(outfile, "%d\n",c);
                //fputc(c, outfile);
                buffer[r++] = c;  r &= (N - 1);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int dec;
    char *s;

    if (argc != 4) {
        printf("Usage: lzss d infile outfile\n\td = decompress\n");
        return 1;
    }
    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D'))
        dec = (*s == 'd' || *s == 'D');
    else {
        printf("? %s\n", s);  return 1;
    }
    if ((infile  = fopen(argv[2], "rb")) == NULL) {
       printf("? %s\n", argv[2]);  return 1;
    }
    if ((outfile = fopen(argv[3], "wb")) == NULL) {
        printf("? %s\n", argv[3]);  return 1;
    }
    
    if (dec) decompress();
    fclose(infile); fclose(outfile); 
    return 0;
}

