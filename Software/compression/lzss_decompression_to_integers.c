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

FILE *outfile;

int inputComp[]={-100,
82,
-20,
21,
72,
-67,
-116,
-8,
99,
49,
-85,
26,
96,
81,
42,
-106,
58,
-91,
-125,
87,
-124,
72,
8,
-67,
-125,
9,
25,
-79,
-58,
-93,
86,
10,
92,
106,
-87,
65,
74,
26,
-92,
93,
-96,
1,
-126,
51,
26,
-79,
-119,
5,
18,
-87,
99,
-86,
88,
53,
120,
64,
-32,
-117,
-40,
48,
-111,
-101,
28,
106,
53,
96,
-91,
-58,
-86,
-112};

int compDataArraySize = 70;
int lineNumber =0;

int getbit(int n) /* get n bits */
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

void decompress()
{
    int i, j, k, r, c;

    lineNumber=0;

    //fileToArray();
   //printf("SIZE: %d",compDataArraySize);

    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
    while ((c = getbit(1)) != EOF) {
        if (c) {
            if (lineNumber >= compDataArraySize) break;
            c=getbit(8);
            //printf("+%d\n",c);
            fprintf(outfile, "%d\n",c);
            //fputc(c, outfile);
            buffer[r++] = c;  r &= (N - 1);
        } else {
            i = getbit(EI);
            j = getbit(EJ);
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
    
    
    if (denc) decompress();
    fclose(outfile); 
    return 0;
}

