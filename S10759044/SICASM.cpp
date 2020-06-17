﻿#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector> 
#include <sstream>
#include <iomanip> 

using namespace std;

struct node {
	char symbol[20];
	int value;
	struct node* next;
};

struct node* HEAD = NULL;
struct node* TAIL = NULL;
int node_cnt = 0;
int true_address[100] = { 0 };
vector<vector<string> > table;
int index = 0;
string int2str(int i);
FILE *obj, *lst, *stb;
char obj_content[1000000] = { '\0' };
int half_byte_cnt = 0;

void table_check_and_insert(char* token, int cnt, int loc, int mode);
void Push(char symbol[], int symbol_len, int value);
void printList();
void insertionSort();
void sortedInsert(struct node** head_ref, struct node* new_node);
char* ASCII_table(char token[]);
char* op_table(char token[]);
char* travel_list(char token[]);

int main(int argc, char** argv)
{
	char s[80] = { '\0' };
	int first_dot = 1; //第一個點之前
	int start_loc, end_loc;
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
	lst = fopen(output_lst, "w");
	obj = fopen(output_obj, "w");
	stb = fopen(output_stb, "w");
	int mode;
	int loc = -1;
	if (argc == 2)
	{
		mode = 0;
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
		int var_or_subr = 0; //1是var; 2是subr; 3可能是var或subr
		int count = 0;
		int count1 = 0;
		char loc_char[100];
		fgets(StrLine, 50, fp);  //讀取一行
		++cnt;
		strncpy(StrLine_copy, StrLine, 50);
		token = strtok(StrLine, "\t"); /* get the first token */
									   /*
									   char token_no_new_line[20] = { '\0' };
									   strncpy(token_no_new_line, token, 20);
									   if (token_no_new_line[strlen(token_no_new_line) - 1] == '\n')
									   {
									   token_no_new_line[strlen(token_no_new_line) - 1] = '\0';
									   }
									   if (strcmp(token_no_new_line, "AND") == 0 || strcmp(token_no_new_line, "DIV") == 0 || strcmp(token_no_new_line, "J") == 0 || strcmp(token_no_new_line, "JEQ") == 0 || strcmp(token_no_new_line, "JGT") == 0 || strcmp(token_no_new_line, "JLT") == 0 || strcmp(token_no_new_line, "JSUB") == 0 || strcmp(token_no_new_line, "LDA") == 0 || strcmp(token_no_new_line, "LDCH") == 0 || strcmp(token_no_new_line, "LDL") == 0 || strcmp(token_no_new_line, "LDX") == 0 || strcmp(token_no_new_line, "MUL") == 0 || strcmp(token_no_new_line, "OR") == 0 || strcmp(token_no_new_line, "RSUB") == 0 || strcmp(token_no_new_line, "STA") == 0 || strcmp(token_no_new_line, "STCH") == 0 || strcmp(token_no_new_line, "STL") == 0 || strcmp(token_no_new_line, "STSW") == 0 || strcmp(token_no_new_line, "STX") == 0 || strcmp(token_no_new_line, "SUB") == 0)
									   {
									   char* op_string = op_table(token_no_new_line);
									   strcat(obj_content, op_string);
									   half_byte_cnt += 3;
									   if (half_byte_cnt > 30)
									   {
									   strcat(obj_content, "\nT");
									   half_byte_cnt = 0;
									   }
									   }
									   */

		if (cnt == 1)
			fprintf(obj, "H%s	", token);

		if (cnt == 2) {
			string loc_string = int2str(loc);
			table[1][0].clear();
			table[1][0].append(loc_string);

			//更改cnt == 1在linked-list第一個node->value
			HEAD->value = loc;
			char loc_char[10];
			sprintf(loc_char, "T%06X", loc);
			strcat(obj_content, loc_char);
		}
		if (StrLine_copy[0] != '\t' && StrLine_copy[0] != '.') {
			table_check_and_insert(token, cnt, loc, 1);
			var_or_subr = 3; //可能是var或subr，須由下一個被切到的字判斷
		}
		if (StrLine_copy[0] == '.')
		{
			if (first_dot == 1)
			{
				char half_byte_cnt_char[10];
				sprintf(half_byte_cnt_char, "%02X", half_byte_cnt);
				strcat(obj_content, half_byte_cnt_char);
				half_byte_cnt = 0;
				strcat(obj_content, s);
				memset(s, '\0', 80);

				char loc_char[10];
				sprintf(loc_char, "\nT%06X", loc);
				strcat(obj_content, loc_char);
			}
			first_dot = 2;

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
				table_check_and_insert(token, cnt, loc, 2);
				type = -1;
			}
			else if (strcmp(token, "START") == 0) {
				type = 0;
				count = count + 1;
			}
			else if (strcmp(token, "BYTE") == 0) {
				type = 1;

				if (first_dot == 1)
				{
					half_byte_cnt += 3;
					if (half_byte_cnt > 30)
					{
						strcat(obj_content, "\nT");
						half_byte_cnt = 0;
					}
				}
				count = count + 1;
			}
			else if (strcmp(token, "WORD") == 0) {
				type = 2;
				if (first_dot == 1)
				{
					half_byte_cnt += 3;
					if (half_byte_cnt > 30)
					{
						strcat(obj_content, "\nT");
						half_byte_cnt = 0;
					}
				}
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

				fprintf(obj, "%06X", loc);
				start_loc = loc;

				type = -1;
			}
			else if (type == 1) {
				if (token[0] == 'X') {
					loc = loc + 1;
				}
				else if (token[0] == 'C') {//token == "C'EOF'"
					int length = strlen(token) - 4;
					loc = loc + length;
					sprintf(loc_char, "%d", loc);
					char *str = ASCII_table(token);
					strcat(s, str);
				}
				type = -1;
			}
			else if (type == 2) {
				loc = loc + 3;

				if (first_dot == 1)
				{
					token[strlen(token) - 1] = '\0'; //get rid of '\n' of token
					int c2i = atoi(token);
					char format_value[10];
					sprintf(format_value, "%06X", c2i);
					strcat(s, format_value);
				}

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
			if (token != NULL)
				strcat(s, travel_list(token));
			if (first_dot > 1 && token != NULL && (strcmp(token, "AND") == 0 || strcmp(token, "DIV") == 0 || strcmp(token, "J") == 0 || strcmp(token, "JEQ") == 0 || strcmp(token, "JGT") == 0 || strcmp(token, "JLT") == 0 || strcmp(token, "JSUB") == 0 || strcmp(token, "LDA") == 0 || strcmp(token, "LDCH") == 0 || strcmp(token, "LDL") == 0 || strcmp(token, "LDX") == 0 || strcmp(token, "MUL") == 0 || strcmp(token, "OR") == 0 || strcmp(token, "RSUB") == 0 || strcmp(token, "STA") == 0 || strcmp(token, "STCH") == 0 || strcmp(token, "STL") == 0 || strcmp(token, "STSW") == 0 || strcmp(token, "STX") == 0 || strcmp(token, "SUB") == 0))
			{
				half_byte_cnt += 3;
				if (half_byte_cnt <= 30)
				{
					char* op_string = op_table(token);
					strcat(s, op_string);
				}
				else if (half_byte_cnt > 30)
				{
					strcat(obj_content, "\nT");
					half_byte_cnt = 0;
				}
			}

			if (first_dot > 1 && var_or_subr == 3 && strcmp(token, "BYTE") != 0 && strcmp(token, "WORD") != 0 && strcmp(token, "STL") != 0)
			{
				//find the true address of subr, and need to get newline in the obj file

				char half_byte_cnt_char[10];
				sprintf(half_byte_cnt_char, "%02X", half_byte_cnt);
				strcat(obj_content, half_byte_cnt_char);
				half_byte_cnt = 0;
				strcat(obj_content, s);
				memset(s, '\0', 80);

				char loc_char[10];
				sprintf(loc_char, "\nT%06X", loc);
				strcat(obj_content, loc_char);
			}
			var_or_subr = 0;
		}
		if (cnt == 1 && type != 5 && count1 == 1) {
			fprintf(lst, "%05X	", loc);
			fprintf(lst, "%s", StrLine_copy);
		}
		if (count == 0) {
			loc = loc + 3;
		}

		if (cnt == 16)
		{
			break;
		}
	}
	end_loc = loc;
	fprintf(obj, "%06X\n", end_loc - start_loc);

	fprintf(obj, "%s", obj_content);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			cout << setw(7) << right << table[i][j];
		}
		printf("\n");
	}

	insertionSort();
	printList();

	fprintf(stb, "Symbol	Value\n======	======\n");
	struct node *temp = HEAD;
	while (temp != NULL)
	{
		fprintf(stb, "%s	%05X\n", temp->symbol, temp->value);
		temp = temp->next;
	}

	fclose(stb);
	fclose(fp);
	fclose(lst);
	fclose(obj);
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
void table_check_and_insert(char* token, int cnt, int loc, int mode)
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

					if (mode == 1)
					{
						true_address[i] = k;

						char cstr[20];
						strcpy(cstr, table[0][i].c_str());
						int value = std::stoi(table[k][i]);
						Push(cstr, strlen(cstr), value);
					}
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

				if (mode == 1)
				{
					true_address[index] = k;
					char cstr[20];
					strcpy(cstr, table[0][index].c_str());
					int value = std::stoi(table[k][index]);
					Push(cstr, strlen(cstr), value);
				}
				break;
			}
		}
		index++;
	}
}
void Push(char symbol[], int symbol_len, int value)
{
	if (HEAD == NULL) {
		HEAD = (struct node*)malloc(sizeof(struct node));
		for (int i = 0; i < symbol_len; i++)
		{
			HEAD->symbol[i] = symbol[i];
		}
		HEAD->symbol[symbol_len] = '\0';
		HEAD->value = value;
		HEAD->next = NULL;
		TAIL = HEAD;
	}
	else {
		struct node* ptr = (struct node*)malloc(sizeof(struct node));
		for (int i = 0; i < symbol_len; i++)
		{
			ptr->symbol[i] = symbol[i];
		}
		ptr->symbol[symbol_len] = '\0';
		ptr->value = value;
		ptr->next = NULL;
		TAIL->next = ptr;
		TAIL = ptr;
	}
	node_cnt++;
}
void printList()
{
	struct node *temp = HEAD;
	while (temp != NULL)
	{
		printf("%s	%d\n", temp->symbol, temp->value);
		temp = temp->next;
	}
}
// function to sort a singly linked list using insertion sort 
void insertionSort()
{
	// Initialize sorted linked list 
	struct node *sorted = NULL;

	// Traverse the given linked list and insert every 
	// node to sorted 
	struct node *current = HEAD;
	while (current != NULL)
	{
		// Store next for next iteration 
		struct node *next = current->next;

		// insert current in sorted linked list 
		sortedInsert(&sorted, current);

		// Update current 
		current = next;
	}

	// Update HEAD to point to sorted linked list 
	HEAD = sorted;
}

/* function to insert a new_node in a list. Note that this
function expects a pointer to head_ref as this can modify the
head of the input linked list (similar to push())*/
void sortedInsert(struct node** head_ref, struct node* new_node)
{
	struct node* current;
	/* Special case for the head end */
	if (*head_ref == NULL || (*head_ref)->value > new_node->value)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		/* Locate the node before the point of insertion */
		current = *head_ref;
		while (current->next != NULL && current->next->value <= new_node->value)
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

char* ASCII_table(char token[])
{
	//C'EOF'\n
	//from token[2] to token[strlen(token) - 3]
	char str[80] = { '\0' };
	for (int i = 2; i <= strlen(token) - 3; i++)
	{
		if (token[i] == '0')
		{
			strcat(str, "30");
		}
		else if (token[i] == '1')
		{
			strcat(str, "31");
		}
		else if (token[i] == '2')
		{
			strcat(str, "32");
		}
		else if (token[i] == '3')
		{
			strcat(str, "33");
		}
		else if (token[i] == '4')
		{
			strcat(str, "34");
		}
		else if (token[i] == '5')
		{
			strcat(str, "35");
		}
		else if (token[i] == '6')
		{
			strcat(str, "36");
		}
		else if (token[i] == '7')
		{
			strcat(str, "37");
		}
		else if (token[i] == '8')
		{
			strcat(str, "38");
		}
		else if (token[i] == '9')
		{
			strcat(str, "39");
		}
		else if (token[i] == 'A')
		{
			strcat(str, "41");
		}
		else if (token[i] == 'B')
		{
			strcat(str, "42");
		}
		else if (token[i] == 'C')
		{
			strcat(str, "43");
		}
		else if (token[i] == 'D')
		{
			strcat(str, "44");
		}
		else if (token[i] == 'E')
		{
			strcat(str, "45");
		}
		else if (token[i] == 'F')
		{
			strcat(str, "46");
		}
		else if (token[i] == 'G')
		{
			strcat(str, "47");
		}
		else if (token[i] == 'H')
		{
			strcat(str, "48");
		}
		else if (token[i] == 'I')
		{
			strcat(str, "49");
		}
		else if (token[i] == 'J')
		{
			strcat(str, "4A");
		}
		else if (token[i] == 'K')
		{
			strcat(str, "4B");
		}
		else if (token[i] == 'L')
		{
			strcat(str, "4C");
		}
		else if (token[i] == 'M')
		{
			strcat(str, "4D");
		}
		else if (token[i] == 'N')
		{
			strcat(str, "4E");
		}
		else if (token[i] == 'O')
		{
			strcat(str, "4F");
		}
		else if (token[i] == 'P')
		{
			strcat(str, "50");
		}
		else if (token[i] == 'Q')
		{
			strcat(str, "51");
		}
		else if (token[i] == 'R')
		{
			strcat(str, "52");
		}
		else if (token[i] == 'S')
		{
			strcat(str, "53");
		}
		else if (token[i] == 'T')
		{
			strcat(str, "54");
		}
		else if (token[i] == 'U')
		{
			strcat(str, "55");
		}
		else if (token[i] == 'V')
		{
			strcat(str, "56");
		}
		else if (token[i] == 'W')
		{
			strcat(str, "57");
		}
		else if (token[i] == 'X')
		{
			strcat(str, "58");
		}
		else if (token[i] == 'Y')
		{
			strcat(str, "59");
		}
		else if (token[i] == 'Z')
		{
			strcat(str, "5A");
		}
	}
	return str;
}

char* op_table(char token[])
{
	if (strcmp(token, "AND") == 0)
	{
		return "40";
	}
	else if (strcmp(token, "DIV") == 0)
	{
		return "24";
	}
	else if (strcmp(token, "J") == 0)
	{
		return "3C";
	}
	else if (strcmp(token, "JEQ") == 0)
	{
		return "30";
	}
	else if (strcmp(token, "JGT") == 0)
	{
		return "34";
	}
	else if (strcmp(token, "JLT") == 0)
	{
		return "38";
	}
	else if (strcmp(token, "JSUB") == 0)
	{
		return "48";
	}
	else if (strcmp(token, "LDA") == 0)
	{
		return "00";
	}
	else if (strcmp(token, "LDCH") == 0)
	{
		return "50";
	}
	else if (strcmp(token, "LDL") == 0)
	{
		return "08";
	}
	else if (strcmp(token, "LDX") == 0)
	{
		return "04";
	}
	else if (strcmp(token, "MUL") == 0)
	{
		return "20";
	}
	else if (strcmp(token, "OR") == 0)
	{
		return "44";
	}
	else if (strcmp(token, "RSUB") == 0)
	{
		return "4C";
	}
	else if (strcmp(token, "STA") == 0)
	{
		return "0C";
	}
	else if (strcmp(token, "STCH") == 0)
	{
		return "54";
	}
	else if (strcmp(token, "STL") == 0)
	{
		return "14";
	}
	else if (strcmp(token, "STSW") == 0)
	{
		return "E8";
	}
	else if (strcmp(token, "STX") == 0)
	{
		return "10";
	}
	else if (strcmp(token, "SUB") == 0)
	{
		return "1C";
	}
}

char* travel_list(char token[])
{
	char *token_copy = (char *)malloc(strlen(token) + 1);
	strcpy(token_copy, token);
	if (token_copy != NULL)
	{
		if (token_copy[strlen(token_copy) - 1] == '\n')
		{
			token_copy[strlen(token_copy) - 1] = '\0';
		}
	}
	struct node *temp = HEAD;
	while (temp != NULL)
	{
		if (token_copy != NULL && strcmp(temp->symbol, token_copy) == 0)
		{
			char format_val[20];
			sprintf(format_val, "%04X", temp->value);
			return format_val;
		}
		temp = temp->next;
	}
	return "";
}