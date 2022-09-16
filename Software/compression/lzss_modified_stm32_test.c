/* 
 * This is a modified version of the original LZSS encoder-decoder (Haruhiko Okumura; public domain) 
 * Instead of taking in a file and array of values is used as input.
 * Instead of printing to a file, the compressed data is store in an array.
 */

#include <stdio.h>
#include <stdlib.h>

#define EI 11  /* typically 10..13 */
#define EJ  5  /* typically 4..5 */
#define P   1  /* If match length <= P then output one character */
#define N (1 << EI)  /* buffer size */
#define F ((1 << EJ) + 1)  /* lookahead buffer size */

int bit_buffer = 0, bit_mask = 128;
unsigned long codecount = 0, textcount = 0;
unsigned char buffer[N * 2];

/** 
 * This array stores the encoded bit_buffers of all the data. The size needs to be chosen based on the number of bits of data.
 * For the STM32F0 implementation, the size will need to be determine based on available space on the STM. This will likely be 
 * through trial and error
 */
int compressed[4970000]; // needs to be atleast the size of the input data (minimum). this size should be the limit of data stored at any one time
int compressedBits =0;

/**
* This is the mock input array of data to be compressed
*/
char inputArray[] = "0.054000001,6,0.0024,-0.0006,3.856600046,-0.061000001,-0.061000001,0,34.83589935\n0.066,7,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n0.07,8,0.0048,-0.003,4.239200115,0,-0.061000001,0,34.84180069\n0.082999997,9,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n0.085000001,10,0.0006,-0.006,4.485300064,0,-0.061000001,0,34.83589935\n0.101999998,11,-0.0006,-0.0048,4.633200169,-0.061000001,-0.061000001,0,34.81240082\n0.109999999,12,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n0.112999998,13,0,-0.0042,4.732600212,-0.061000001,-0.061000001,0,34.82410049\n0.131999999,14,0.003,-0.003,4.794199944,0,-0.061000001,0,34.83000183\n0.140000001,15,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.143999994,16,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.156000003,17,-0.0006,-0.003,4.829599857,-0.061000001,-0.061000001,0,34.83000183\n0.164000005,18,0,-0.006,4.858300209,-0.061000001,-0.061000001,0,34.81240082\n0.172999993,19,-0.003,-0.0054,4.869699955,-0.061000001,-0.061000001,0,34.81240082\n0.174999997,20,-0.003,-0.0054,4.869699955,-0.061000001,-0.061000001,0,34.81240082\n0.187999994,21,-0.0042,-0.0054,4.879799843,0,-0.061000001,-0.061000001,34.84180069\n0.192000002,22,-0.0042,-0.0054,4.879799843,0,-0.061000001,-0.061000001,34.84180069\n0.211999997,23,-0.0054,-0.006,4.889999866,0,-0.061000001,0,34.81240082\n0.221000001,24,-0.0072,-0.0042,4.89659977,-0.061000001,-0.061000001,0,34.83000183\n0.224999994,25,-0.0072,-0.0042,4.89659977,-0.061000001,-0.061000001,0,34.83000183\n0.241999999,26,-0.006,-0.0048,4.901400089,-0.061000001,-0.061000001,0,34.83000183\n0.250999987,27,-0.003,-0.0012,4.902599812,-0.061000001,-0.061000001,-0.061000001,34.82410049\n0.254999995,28,-0.003,-0.0012,4.902599812,-0.061000001,-0.061000001,-0.061000001,34.82410049\n0.268000007,29,-0.0036,-0.0036,4.90320015,-0.061000001,-0.061000001,0,34.83589935\n0.275999993,30,-0.003,0,4.899600029,0,-0.061000001,0,34.83000183\n0.280000001,31,-0.003,0,4.899600029,0,-0.061000001,0,34.83000183\n0.301999986,32,-0.003,-0.0006,4.900800228,0,-0.061000001,0,34.83000183\n0.31099999,33,-0.0006,-0.0006,4.901400089,-0.061000001,-0.061000001,-0.061000001,34.84180069\n0.31400001,34,-0.0006,-0.0006,4.901400089,-0.061000001,-0.061000001,-0.061000001,34.84180069\n0.326000005,35,0,-0.0006,4.901400089,-0.061000001,-0.061000001,0,34.85350037\n0.330000013,36,0,-0.0006,4.901400089,-0.061000001,-0.061000001,0,34.85350037";

void error(void)
{
    //printf("Output error\n");  
    exit(1);
}

/**
 * This method has been added to store the compression encoded bits in an array that will be passed to the encryption algorithm.
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

void encode(void) // should bee modified to take in value
{
    int i, j, f1, x, y, r, s, bufferend, c;
    
    int counter = 0;
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    for (i = N - F; i < N * 2; i++) {
        if ( counter >= sizeof(inputArray)) break;
        c = inputArray[counter];
        buffer[i] = c;  counter++;
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
                if ( counter >= sizeof(inputArray)) break;
                c = inputArray[counter];
                buffer[bufferend++] = c;  counter++;
                //textcount++;
            }
        }
    }
    FILE *f = fopen("testcomp", "w+");
    for (int jk=0;jk<compressedBits;jk++){
        fputc(compressed[jk],f);
    }
    //fprintf(f, "%s",compressed);
    fclose(f);
}

int compressedIndex = 0;
int getbit(int n) /* get n bits */
{
    int i, x;
    static int buf, mask = 0;
    
    x = 0;
    for (i = 0; i < n; i++) {
        if (mask == 0) {
            if (compressedIndex<compressedBits) break;
            buf = compressed[compressedIndex];
            compressedIndex++;
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
    int i, j, k, r, c;
    
    compressedIndex=0; //reset this
    
    for (i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
    while ((c = getbit(1)) != EOF) {
        if (c) {
            if ((c = getbit(8)) == EOF) break;
            printf("%c",c);
            buffer[r++] = c;  r &= (N - 1);
        } else {
            if ((i = getbit(EI)) == EOF) break;
            if ((j = getbit(EJ)) == EOF) break;
            for (k = 0; k <= j + 1; k++) {
                c = buffer[(i + k) & (N - 1)];
                printf("%c",c);
                buffer[r++] = c;  r &= (N - 1);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int enc;
    char *s;
    
    if (argc != 2) {
        printf("Usage: lzss e/d \n\te = encode\td = decode\n");
        return 1;
    }
    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D' || *s == 'e' || *s == 'E'))
        enc = (*s == 'e' || *s == 'E');
    else {
        printf("? %s\n", s);  return 1;
    }
   
    if (enc) encode(); else decode();
    return 0;
}