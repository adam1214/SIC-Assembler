#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector> 
#include <sstream>

using namespace std;
string int2str(int i);

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
	int loc = -1;
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
	if ((fp = fopen(filename, "r")) == NULL) //判斷檔案是否存在及可讀
	{
		printf("error!");
		system("pause");
		return -1;
	}
	fprintf(lst, "Loc.	Source statement\n=====	==================================\n");
	int cnt = 0;
	int type = -1;
	int index = 0;
	int j = 0;
	string stbloc;
	vector<vector<string> > table;
	table.resize(100);
	for (int i = 0; i < 100; i++)
	{
		table[i].resize(100);
	}
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			table[i][j].clear();
			table[i][j].append("0");
		}
	}
	while (!feof(fp))
	{	
		int count = 0;
		int count1 = 0;
		char loc_char[100];
		fgets(StrLine, 50, fp);  //讀取一行
		++cnt;
		strncpy(StrLine_copy, StrLine, 50);
		token = strtok(StrLine, "\t"); /* get the first token */
		if (StrLine_copy[0] != '\t' && StrLine_copy[0]!='.') {
			string token_string(token);
			table[0][index].clear();
			table[0][index].append(token_string);
			if (cnt > 1)
			{
				for (int k = 1; k < 100; k++)
				{
					if ("0" == table[k][index])
					{
						string loc_string = int2str(loc);
						table[k][index].clear();
						table[k][index].append(loc_string);
						break;
					}
				}
			}
			index++;
		}
		if (StrLine_copy[0] == '.')
		{
			type = 5;
			fprintf(lst, "\t%s", StrLine_copy);
			count = count + 1;

		}
		if (cnt > 1 && type != 5 && type != 6)
		{
			if (strcmp(token, "END") != 0)
			{
				fprintf(lst, "%05X	", loc);
			}
			fprintf(lst, "%s", StrLine_copy);
		}
		type = -1;
		while (token != NULL)
		{	
			
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
				type = -1;
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
		if (cnt == 1 && type != 5 && count1 == 1) {
			fprintf(lst, "%05X	", loc);
			fprintf(lst, "%s", StrLine_copy);
		}
		if (count == 0) {
			loc = loc + 3; 
		}
		//if (cnt == 52){
		//break;
		//}
	}

	//fclose(fp);
	//fclose(lst);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << table[i][j] << " ";
		}
		printf("\n");
	}
	system("pause");
	return 0;
}
string int2str(int i) 
{
	string s;
	stringstream ss(s);
	ss << i;
	return ss.str();
}