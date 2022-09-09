#include <stdio.h>
#include <stdint.h>
//main function copied from https://www.w3resource.com/c-programming-exercises/file-handling/c-file-handling-exercise-13.php
void main()
{
	char fname[20];
	uint8_t ch[256];
	FILE *fpts, *fptt;
	int count = 0;

	printf("\n\n Encrypt a text file :\n");
	printf("--------------------------\n");

	printf(" Input the name of file to encrypt : ");
	scanf("%s",fname);

	fpts=fopen(fname, "r");
	if(fpts==NULL)
	{
		printf(" File does not exists or error in opening..!!");
		exit(1);
	}
	fptt=fopen("encrypted.txt", "w");
	if(fptt==NULL)
	{
		printf(" Error in creation of file encrypted.txt ..!!");
		fclose(fpts);
		exit(2);
	}
	while(1)
	{
		if(ch[count]==EOF)
		{
			break;
		}
		else
		{
            ch[count]=fgetc(fpts);
            count++;
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

            fputc(ciphertext_buf, fptt);
	}
	fclose(fpts);
	fclose(fptt);
	//fpts=fopen(fname, "w");
	if(fpts==NULL)
	{
		printf(" File does not exists or error in opening..!!");
		exit(3);
	}
	fptt=fopen("encrypted.txt", "r");
	if(fptt==NULL)
	{
		printf(" File does not exists or error in opening..!!");
		fclose(fpts);
		exit(4);
	}
	while(1)
	{
		ch=fgetc(fptt);
		if(ch==EOF)
		{
			break;
		}
		else
		{
			fputc(ch, fptt);
		}
	}
	printf(" File %s successfully encrypted ..!!\n\n", fname);
	fclose(fpts);
	fclose(fptt);
}
