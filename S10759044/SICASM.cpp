#include <cstdio> //for c lib <stdio.h>
#include <cstdlib> //for c lib <stdlib.h>
#include <iostream>
#include <cstring>
int main(int argc, char** argv)
{	
	int mode;
	if (argc == 2)
	{
		mode = 0;
	}
	else if (argv[2][1] == 's')
	{
		mode = 1;
	}
	else if (argv[2][1] == 't')
	{
		mode = 2;
	}
	else if (argv[2][1] == 'a')
	{
		mode = 3;
	}

	//printf("%d\n", argc);
	//printf("%s\n", argv[1]);
	char filename[] = "test.asm"; //�ɦW
	FILE *fp;
	char StrLine[50];             //�C��̤jŪ�����r����
	if((fp = fopen(filename, "r")) == NULL) //�P�_�ɮ׬O�_�s�b�ΥiŪ
	{
		printf("error!");
		system("pause");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(StrLine, 50, fp);  //Ū���@��
		printf("%s\n", StrLine); //��X
		//StrLine�����r�� (strtok ���X while loop)

	}
	fclose(fp);      //�����ɮ�
	system("pause"); //<cstdlib>
	return 0;
}