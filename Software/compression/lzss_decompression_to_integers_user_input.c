/*
 * This is a modified version of the original LZSS decoder (Haruhiko Okumura; public domain)
 * Instead of taking in a file an array of values is used as input.
 * The decompressed data is then printed to a file.
 */

#include <stdio.h>
#include <stdlib.h>

#define EI 6  /* typically 10..13 */
#define EJ  5  /* typically 4..5 */
#define P   1  /* If match length <= P then output one character */
#define N (1 << EI)  /* buffer size */
#define F ((1 << EJ) + 1)  /* lookahead buffer size */

int bit_buffer = 0, bit_mask = 128;
int buffer[N * 2];

FILE *infile, *outfile;

int lineNumber =0;
int compDataArraySize = 0;

int getbit(int n, int inputComp[]) /* get n bits */
{
    int i, x;
    static int buf, mask = 0;

    x = 0;
    for (i = 0; i < n; i++) {
        if (mask == 0) {
            if (lineNumber>=compDataArraySize) break;

            //printf("%d; %d\n",lineNumber,inputComp[lineNumber]);

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

void decompress(int inputComp[])
{
    int i, j, k, r, c;

    lineNumber=0;

    //fileToArray();
   //printf("SIZE: %d",compDataArraySize);

    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
    while ((c = getbit(1, inputComp)) != EOF) {
        if (c) {
            if (lineNumber >= compDataArraySize) break;
            c=getbit(8,inputComp);
            //printf("+%d\n",c);
            fprintf(outfile, "%d\n",c);
            //fputc(c, outfile);
            buffer[r++] = c;  r &= (N - 1);
        } else {
            i = getbit(EI, inputComp);
            j = getbit(EJ, inputComp);
            if (lineNumber>=compDataArraySize) break;
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
    int denc;
    char *s;

    if (argc != 3) {
        printf("Usage: lzss d arrSize outfile\n\td = decode\n");
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
    
    printf("Number of elements?\n");
    scanf("%d", &compDataArraySize);
    
    int inputComp[compDataArraySize];
    printf("Enter the input elements:\n");
    for (int i = 0; i < compDataArraySize; i++) {
        scanf("%d", &inputComp[i]);
    }
    
    printf("The elements are:\n");
    for (int i = 0; i < compDataArraySize; i++) {
        printf("%d,", inputComp[i]);
    }
    
    if (denc) decompress(inputComp);
    fclose(outfile); fclose(infile);
    return 0;
}

