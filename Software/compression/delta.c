/* LZSS encoder-decoder (Haruhiko Okumura; public domain) */

#include <stdio.h>
#include <stdlib.h>

#define EI 11  /* typically 10..13 */
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

void encode(void)
{
    int i, j, f1, x, y, r, s, bufferend, c;
    
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    for (i = N-F; i < N*2; i++) {
        if ((c = fget(infile)) == EOF) break;
        buffer[i] = c;  textcount++;
    }
    bufferend = i;

    unsigned char last = 0;
    for (int i = 0; i < bufferend; i++)
    {
        unsigned char current = buffer[i];
        printf("%c  ",current);
        buffer[i] = current - last;
        last = current;
        printf("%c   \n",last);
        printf("%c\n",current-last);
        codecount++;
    }

    flush_bit_buffer();
    printf("compressed\n");
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

void decode(void)
{
    int i, j, k, r, c, bufferend;
    
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    bufferend = i;
    while ((c = getbit(1)) != EOF) {
        unsigned char last = 0;
        for (int i = 0; i < bufferend; i++)
        {
            unsigned char delta = buffer[i];
            buffer[i] = delta + last;
            last = buffer[i];
        }
    }
}

int main(int argc, char *argv[])
{
    int enc;
    char *s;
    
    if (argc != 4) {
        printf("Usage: lzss e/d infile outfile\n\te = encode\td = decode\n");
        return 1;
    }
    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D' || *s == 'e' || *s == 'E'))
        enc = (*s == 'e' || *s == 'E');
    else {
        printf("? %s\n", s);  return 1;
    }
    if ((infile  = fopen(argv[2], "rb")) == NULL) {
        printf("? %s\n", argv[2]);  return 1;
    }
    if ((outfile = fopen(argv[3], "wb")) == NULL) {
        printf("? %s\n", argv[3]);  return 1;
    }
    if (enc) encode();  else decode();
    fclose(infile);  fclose(outfile);
    return 0;
}