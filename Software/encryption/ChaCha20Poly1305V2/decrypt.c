
//main function copied from https://www.w3resource.com/c-programming-exercises/file-handling/c-file-handling-exercise-13.php
void main()
{
	char fname[20], ch;
	FILE *fpts, *fptt;

	printf("\n\n Encrypt a text file :\n");
	printf("--------------------------\n");

	printf(" Input the name of file to decrypt : ");
	scanf("%s",fname);

	fpts=fopen(fname, "r");
	if(fpts==NULL)
	{
		printf(" File does not exists or error in opening..!!");
		exit(1);
	}
	fptt=fopen("decrypted.txt", "w");
	if(fptt==NULL)
	{
		printf(" Error in creation of file decrypted.txt ..!!");
		fclose(fpts);
		exit(2);
	}
	while(1)
	{
		ch=fgetc(fpts);
		if(ch==EOF)
		{
			break;
		}
		else

		{

                fputc(actual_plaintext, fptt);
		}
	}
	fclose(fpts);
	fclose(fptt);
	fptt=fopen("decrypted.txt", "r");
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
	printf(" File %s successfully decrypted ..!!\n\n", fname);
	fclose(fptt);
}

