#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector> 
#include <sstream>
#include <iomanip> 

using namespace std;

vector<vector<string> > table;
int index = 0;
string int2str(int i);
void table_check_and_insert(char* token, int cnt, int loc);
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
	int j = 0;
	string stbloc;
	table.resize(10);
	for (int i = 0; i < 10; i++)
	{
		table[i].resize(100);
	}
	for (int i = 0; i < 10; i++)
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
		if (cnt == 2) {
			string loc_string = int2str(loc);
			table[1][0].clear();
			table[1][0].append(loc_string);
		}
		if (StrLine_copy[0] != '\t' && StrLine_copy[0] != '.') {
			table_check_and_insert(token, cnt, loc);
			/*
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
			*/
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
			if (strcmp(token, "JSUB") == 0 || strcmp(token, "JGT") == 0 || strcmp(token, "JLT") == 0 || strcmp(token, "JEQ") == 0) 
			{
				type = 7;
			}
			else if (type == 7) 
			{
				table_check_and_insert(token, cnt, loc);
				type = -1;
			}
			else if (strcmp(token, "START") == 0) {
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
		for (int j = 0; j < 17; j++)
		{
			cout << setw(7) << right << table[i][j];
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
void table_check_and_insert(char* token, int cnt, int loc)
{
	int exist_or_not = 0;
	char dest[10] = { 0 };
	strncpy(dest, token, strlen(token));
	for (int i = 0; i < 100; i++)//第一列陣列橫向搜索
	{
		if (token[strlen(token) - 1] == '\n')
		{
			memset(dest, '\0', 10);
			strncpy(dest, token, strlen(token) - 1);
		}
		if (table[0][i] == dest)//如果已經有symbol就填loc
		{
			for (int k = 1; k < 10; k++)
			{
				if ("0" == table[k][i])
				{
					string loc_string = int2str(loc);
					table[k][i].clear();
					table[k][i].append(loc_string);
					exist_or_not = 1;
					//cout << loc_string << endl;
					break;
				}
			}
		}
		if (exist_or_not == 1)
		{
			break;
		}
	}
	if (exist_or_not == 0) //如果還未出現此symbol先填symbol再填loc
	{
		//cout << "not exist in table yet:" << dest << " " << cnt << endl;
		string token_string(dest);
		table[0][index].clear();
		table[0][index].append(token_string);
		for (int k = 1; k < 10; k++)
		{
			if ("0" == table[k][index])
			{
				string loc_string = int2str(loc);
				table[k][index].clear();
				table[k][index].append(loc_string);
				break;
			}
		}
		index++;
	}
}