#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "chacha.h"
#include "chachapoly_aead.h"
#include "poly1305.h"

FILE *infile, *outfile;

void encrypt(void)
{
    uint8_t ch[256];
    int count = 0;

    while (1) {
    if(ch[count]==EOF)
		{
			break;
		}
		else
		{
            ch[count]=fgetc(infile);
            count++;
		}
    }
    struct chachapolyaead_ctx aead_ctx;
            uint32_t seqnr = 0;
            uint32_t seqnr_aad = 0;
            int pos_aad = 0;
            uint8_t aead_k_1[64] = {
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
            uint8_t aead_k_2[64] = {
                0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

            uint8_t ciphertext_buf[255 + 16] = {0};
            chacha20poly1305_init(&aead_ctx, aead_k_1, 32, aead_k_2, 32);
            chacha20poly1305_crypt(&aead_ctx, seqnr, seqnr_aad, pos_aad, ciphertext_buf, 300, ch, 255, 1);

            char test[3] = {'a', 'b', 'c'};
            fputc(test, outfile);

            fclose(outfile);
            fclose(infile);

}

void decrypt(void)
{
    uint8_t ch[256];
    int count = 0;

    while (1) {
    if(ch[count]==EOF)
		{
			break;
		}
		else
		{
            ch[count]=fgetc(infile);
            count++;
		}
    }
    struct chachapolyaead_ctx aead_ctx;
            uint32_t seqnr = 0;
            uint32_t seqnr_aad = 0;
            int pos_aad = 0;
            uint8_t aead_k_1[64] = {
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
            uint8_t aead_k_2[64] = {
                0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

            uint8_t ciphertext_buf[255 + 16];
            chacha20poly1305_init(&aead_ctx, aead_k_1, 32, aead_k_2, 32);
            chacha20poly1305_crypt(&aead_ctx, seqnr, seqnr_aad, pos_aad, ciphertext_buf, 300, ch, 255, 0);
            for (int i = 0; i < count; i ++) {
                fputc(ciphertext_buf[i], outfile);

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
    if (enc) encrypt();  else decrypt();
    fclose(infile);  fclose(outfile);
    return 0;
}
