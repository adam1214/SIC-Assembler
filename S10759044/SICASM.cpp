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
	char filename[] = "test.asm"; //檔名
	FILE *fp;
	char StrLine[50];             //每行最大讀取的字元數
	if((fp = fopen(filename, "r")) == NULL) //判斷檔案是否存在及可讀
	{
		printf("error!");
		system("pause");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(StrLine, 50, fp);  //讀取一行
		printf("%s\n", StrLine); //輸出
		//StrLine做切字串 (strtok 結合 while loop)

	}
	fclose(fp);      //關閉檔案
	system("pause"); //<cstdlib>
	return 0;
}