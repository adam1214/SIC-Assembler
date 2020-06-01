// fprintf(fp,"%s",Str); //寫入檔案
// strcat,sprintf,atoi
// just for practicing commit to master. 
#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector> 

using namespace std; 

int main(int argc, char** argv)
{	
	char output_lst[35] = "output/";
	char output_obj[35] = "output/";
	char output_stb[35] = "output/";

	strcat(output_lst, argv[1]);
	strcat(output_obj, argv[1]);
	strcat(output_stb, argv[1]);

	strcat(output_lst, ".lst");
	strcat(output_obj, ".obj");
	strcat(output_stb, ".stb");
	string getstring;
	FILE *ob, *lst, *stb;
	lst = fopen(output_lst, "w");
	int mode;
	long loc = -1;
	if (argc == 2)
	{
		mode = 0;
		ob = fopen("output/test.obj", "w");
	}
	else if (argc == 3 && argv[2][1] == 's')
	{
		mode = 1;
	}
	else if (argc == 3 && argv[2][1] == 't')
	{
		mode = 2;
	}
	else //default mode
	{
		mode = 3;
	}
	char filename[] = "input/test.asm"; //檔名
	FILE *fp;
	char StrLine[50];   //每行最大讀取的字元數
	char StrLine_copy[50];
	char* token = NULL;
	if((fp = fopen(filename, "r")) == NULL) //判斷檔案是否存在及可讀
	{
		printf("error!");
		system("pause");
		return -1;
	}
	fprintf(lst, "Loc.	Source statement\n=====	==================================\n");
	int cnt = 0;

	while (!feof(fp))
	{
		int type = -1;
		int count = 0;
		char loc_char[100];
		fgets(StrLine, 50, fp);  //讀取一行
		++cnt;
		if (StrLine[0] == '.')
		{
			type = 5;
			fprintf(lst, "\t%s", StrLine);
			count = count + 1;
		}
		strncpy(StrLine_copy, StrLine, 50);
		token = strtok(StrLine, "\t");   /* get the first token */
		while (token != NULL)
		{	
			if (cnt > 1 && type != 5)
			{
				fprintf(lst, "%05X	", loc);
				fprintf(lst, "%s", StrLine);
			}
			if (strcmp(token, "START") == 0) {
				type = 0; 
				count = count + 1;
			}
			else if (strcmp(token, "BYTE") == 0) {
				type = 1;
				count = count + 1;
			}
			else if (strcmp(token, "WORD") == 0) {
				type = 2;
				count = count + 1;
			}
			else if (strcmp(token, "RESW") == 0) {
				type = 3;
				count = count + 1;
			}
			else if (strcmp(token, "RESB") == 0) {
				type = 4;
				count = count + 1;
			}
			else if (strcmp(token, "END") == 0) {
				type = 6;
				count = count + 1;
			}
			else if (type == 0) {
				loc = strtol(token, NULL, 16);
				sprintf(loc_char, "%d", loc);
			}
			else if (type == 1) {
				if (token[0] == 'X') {
					loc = loc + 1;
				}
				else if (token[0] == 'C') {
					int length = strlen(token) - 4;
					loc = loc + length;
					sprintf(loc_char, "%d", loc);
				}
				type = -1;
			}
			else if (type == 2) {
				loc = loc + 3;
				type = -1;
			}
			else if (type == 3) {
				int i = atoi(token);
				loc = loc + i * 3;
				type = -1;
			}
			else if (type == 4) {
				int i = atoi(token);
				loc = loc + i;
				type = -1;
			}
			
			/* walk through other tokens */
			token = strtok(NULL, "\t");
			
		}
		
		if (cnt == 1 && type != 5)
		{
			fprintf(lst, "%05X	", loc);
			fprintf(lst, "%s", StrLine_copy);
		}
		if (cnt ==53)
		{
			break;
		}
		
		if (count == 0) {
			loc = loc + 3;
		}
	}

	fclose(fp);
	fclose(lst);
	system("pause");
	return 0;
}