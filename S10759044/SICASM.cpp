#include <iostream>
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
const char* op_table(char token[]);
const char* travel_list(char token[]);

int main(int argc, char** argv)
{
	int at_end_or_not = 0;
	char pre_token[20] = { '\0' };
	char s[80] = { '\0' };
	int first_dot = 1; //第一個點之前
	int first_dot_cnt = 0;
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
	int option = 0;
	int loc = -1;
	if (argc == 3 && argv[2][1] == 's')
	{	
		option = 1;
	}
	else if (argc == 3 && argv[2][1] == 't')
	{
		option = 2;
	}
	else if (argc == 3 && argv[2][1] == 'a')
	{
		option = 3;
	}
	if (option == 1 || option == 3)
	{
		lst = fopen(output_lst, "w");
	}
	obj = fopen(output_obj, "w");
	if (option == 2 || option == 3) 
	{
		stb = fopen(output_stb, "w");
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
	if (option == 1 || option == 3)
	{
		fprintf(lst, "Loc.	Source statement\n=====	==================================\n");
	}
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
	while (true)
	{
		int comment_or_not = 0;
		int var_or_subr = 0; //1是var; 2是subr; 3可能是var或subr; 4必定是subr在定義前被呼叫過，故不換行
		int count = 0;
		int count1 = 0;
		char loc_char[100];
		fgets(StrLine, 50, fp);  //讀取一行
		if (feof(fp))
			break;
		++cnt;
		if (cnt > 1)
			half_byte_cnt += 3;
		strncpy(StrLine_copy, StrLine, 50);
		token = strtok(StrLine, "\t"); /* get the first token */
		if (cnt == 51)
		{
			printf("FFFFFFFFFF = %s\n", token);
		}
		memset(pre_token, '\0', 20);
		if (token != NULL)
			strcpy(pre_token, token);
		if (first_dot > 1 && token != NULL)
		{
			//printf("VVVVVVVVVVVVVVVVVVVV = %s\n", token);
			if (half_byte_cnt > 30)
			{
				//strcat(obj_content, "\nT");
				//half_byte_cnt = 0;
			}
			else
			{
				strcat(s, op_table(token));
			}
		}
		if (token != NULL && strcmp(token, "END") == 0)
		{
			half_byte_cnt -= 3;

			//find the end of the .asm file
			printf("DDDDDDDDDDDDD=%s\n", token);
			char half_byte_cnt_char[10];
			sprintf(half_byte_cnt_char, "%02X", half_byte_cnt);
			strcat(obj_content, half_byte_cnt_char);
			half_byte_cnt = 3;
			strcat(obj_content, s);
			memset(s, '\0', 80);

			strcat(obj_content, "\nE");
			at_end_or_not = 1;
		}
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
			half_byte_cnt -= 3;
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
				first_dot_cnt = cnt;
			}
			first_dot = 2;

			type = 5;
			if (option == 1 || option == 3)
			{
				fprintf(lst, "\t%s", StrLine_copy);
			}
			count = count + 1;
			comment_or_not = 1;
		}
		if (cnt > 1 && type != 5 && type != 6)
		{
			if (option == 1 || option == 3)
			{
				if (strcmp(token, "END") != 0)
				{
					fprintf(lst, "%05X	", loc);
				}
				fprintf(lst, "%s", StrLine_copy);
			}
		}

		type = -1;
		while (token != NULL)
		{
			int op_table_is_check = 0;
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
				count = count + 1;
			}
			else if (strcmp(token, "WORD") == 0) {
				type = 2;
				count = count + 1;
			}
			else if (strcmp(token, "RESW") == 0) {
				type = 3;
				half_byte_cnt -= 3;
				count = count + 1;
			}
			else if (strcmp(token, "RESB") == 0) {
				type = 4;
				half_byte_cnt -= 3;
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
				if (token[0] == 'X') { //X'F1'\n	字串長度為6
					half_byte_cnt = half_byte_cnt + (strlen(token) - 4) / 2 - 3;
					loc = loc + 1;
					char dest[10] = { 0 };
					strncpy(dest, token + 2, strlen(token) - 4);
					strcat(s, dest);
				}
				else if (token[0] == 'C') {//token == "C'EOF'\n"
					half_byte_cnt = half_byte_cnt + (strlen(token) - 4) - 3;
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

				//if (first_dot == 1)
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
			if (var_or_subr == 3 && strcmp(token, "BYTE") != 0 && strcmp(token, "WORD") != 0)
			{
				//還要判斷這個subr是否為先定義還未被呼叫，如果是如此，也不在obj file換行
				for (int i = 0; i < 100; i++)
				{
					char cstr[20];
					strcpy(cstr, table[0][i].c_str());

					if (strcmp(cstr, pre_token) == 0)
					{
						if (true_address[i] > 1)
						{
							var_or_subr = 4;
							break;
						}
					}
				}
			}
			if (cnt > first_dot_cnt + 1 && first_dot > 1 && var_or_subr == 4)
			{
				//find the true address of subr, and need to get newline in the obj file
				//printf("SSSSSSSSSSSSS=%s\n", pre_token);
				char half_byte_cnt_char[10];
				sprintf(half_byte_cnt_char, "%02X", half_byte_cnt - 3);
				strcat(obj_content, half_byte_cnt_char);
				half_byte_cnt = 3;
				strcat(obj_content, s);
				memset(s, '\0', 80);

				strcat(obj_content, "\nT");
				int is_break = 0;
				for (int k = 0; k < 100; k++) //橫向搜索第0列table
				{
					if (table[0][k] == pre_token)
					{
						for (int m = 1; m < 10; m++)
						{
							if (stoi(table[m][k]) == 0)
							{
								is_break = 1;
								break;
							}
							if (stoi(table[m + 1][k]) != 0)
							{
								int value = std::stoi(table[m][k]);
								sprintf(loc_char, "%06X02%04X\nT", ++value, loc);
								strcat(obj_content, loc_char);
							}
						}
					}
					if (is_break == 1)
					{
						break;
					}
				}

				sprintf(loc_char, "%06X", loc);
				strcat(obj_content, loc_char);
				strcat(s, op_table(token));
			}
			else if (comment_or_not == 0 && first_dot > 1 && token != NULL && (strcmp(token, "AND") == 0 || strcmp(token, "DIV") == 0 || strcmp(token, "J") == 0 || strcmp(token, "JEQ") == 0 || strcmp(token, "JGT") == 0 || strcmp(token, "JLT") == 0 || strcmp(token, "JSUB") == 0 || strcmp(token, "LDA") == 0 || strcmp(token, "LDCH") == 0 || strcmp(token, "LDL") == 0 || strcmp(token, "LDX") == 0 || strcmp(token, "MUL") == 0 || strcmp(token, "OR") == 0 || strcmp(token, "RSUB") == 0 || strcmp(token, "STA") == 0 || strcmp(token, "STCH") == 0 || strcmp(token, "STL") == 0 || strcmp(token, "STSW") == 0 || strcmp(token, "STX") == 0 || strcmp(token, "SUB") == 0 || strcmp(token, "COMP") == 0 || strcmp(token, "RD") == 0 || strcmp(token, "TD") == 0 || strcmp(token, "TIX") == 0 || strcmp(token, "WD") == 0 || strcmp(token, "AND\n") == 0 || strcmp(token, "DIV\n") == 0 || strcmp(token, "J\n") == 0 || strcmp(token, "JEQ\n") == 0 || strcmp(token, "JGT\n") == 0 || strcmp(token, "JLT\n") == 0 || strcmp(token, "JSUB\n") == 0 || strcmp(token, "LDA\n") == 0 || strcmp(token, "LDCH\n") == 0 || strcmp(token, "LDL\n") == 0 || strcmp(token, "LDX\n") == 0 || strcmp(token, "MUL\n") == 0 || strcmp(token, "OR\n") == 0 || strcmp(token, "RSUB\n") == 0 || strcmp(token, "STA\n") == 0 || strcmp(token, "STCH\n") == 0 || strcmp(token, "STL\n") == 0 || strcmp(token, "STSW\n") == 0 || strcmp(token, "STX\n") == 0 || strcmp(token, "SUB\n") == 0 || strcmp(token, "COMP\n") == 0 || strcmp(token, "RD\n") == 0 || strcmp(token, "TD\n") == 0 || strcmp(token, "TIX\n") == 0 || strcmp(token, "WD\n") == 0))
			{

				op_table_is_check = 1;
				if (half_byte_cnt > 30)
				{
					//strcat(obj_content, "\nT");
					//half_byte_cnt = 0;
				}
				else
				{
					strcat(s, op_table(token));
				}
			}
			else if (type != 1 && type != 2 && comment_or_not == 0 && token != NULL && strcmp(token, "WORD") != 0 && strcmp(token, "BYTE") != 0 && first_dot > 1 && op_table_is_check == 0)
			{
				if (strcmp(travel_list(token), "0000") == 0 && (cnt == 24 || cnt == 25))
					printf("FFFFFFFFFFFFFF=%s\n", token);
				strcat(s, travel_list(token));
				if (at_end_or_not == 1)
				{
					strcat(obj_content, "00");
					strcat(obj_content, travel_list(token));
				}
			}
			var_or_subr = 0;
			memset(pre_token, '\0', 20);
			if (token != NULL)
				strcpy(pre_token, token);
		}
		if (cnt == 1 && type != 5 && count1 == 1) {
			if (option == 1 || option == 3)
			{
				fprintf(lst, "%05X	", loc);
				fprintf(lst, "%s", StrLine_copy);
			}
		}
		if (count == 0) {
			loc = loc + 3;
		}

		//if (cnt == 49)
		//{
		//break;
		//}
	}
	printf("end_loc = %X\n", loc);
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
	if (option == 2 || option == 3)
	{
		fprintf(stb, "Symbol	Value\n======	======\n");
	}
	struct node *temp = HEAD;
	while (temp != NULL)
	{	
		if (option == 2 || option == 3) 
		{
			fprintf(stb, "%s	%05X\n", temp->symbol, temp->value);
			temp = temp->next;
		}
	}
	
	if (option == 2 || option == 3)
	{
		fclose(stb);
	}
	fclose(fp);
	if (option == 1 || option == 3)
	{
		fclose(lst);
	}
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
	static char str[80] = { '\0' };
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

const char* op_table(char token[])
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
	if (strcmp(token_copy, "AND") == 0)
	{
		free(token_copy);
		return "40";
	}
	else if (strcmp(token_copy, "DIV") == 0)
	{
		free(token_copy);
		return "24";
	}
	else if (strcmp(token_copy, "J") == 0)
	{
		free(token_copy);
		return "3C";
	}
	else if (strcmp(token_copy, "JEQ") == 0)
	{
		free(token_copy);
		return "30";
	}
	else if (strcmp(token_copy, "JGT") == 0)
	{
		free(token_copy);
		return "34";
	}
	else if (strcmp(token_copy, "JLT") == 0)
	{
		free(token_copy);
		return "38";
	}
	else if (strcmp(token_copy, "JSUB") == 0)
	{
		free(token_copy);
		return "48";
	}
	else if (strcmp(token_copy, "LDA") == 0)
	{
		free(token_copy);
		return "00";
	}
	else if (strcmp(token_copy, "LDCH") == 0)
	{
		free(token_copy);
		return "50";
	}
	else if (strcmp(token_copy, "LDL") == 0)
	{
		free(token_copy);
		return "08";
	}
	else if (strcmp(token_copy, "LDX") == 0)
	{
		free(token_copy);
		return "04";
	}
	else if (strcmp(token_copy, "MUL") == 0)
	{
		free(token_copy);
		return "20";
	}
	else if (strcmp(token_copy, "OR") == 0)
	{
		free(token_copy);
		return "44";
	}
	else if (strcmp(token_copy, "RSUB") == 0)
	{
		free(token_copy);
		return "4C0000";
	}
	else if (strcmp(token_copy, "STA") == 0)
	{
		free(token_copy);
		return "0C";
	}
	else if (strcmp(token_copy, "STCH") == 0)
	{
		free(token_copy);
		return "54";
	}
	else if (strcmp(token_copy, "STL") == 0)
	{
		free(token_copy);
		return "14";
	}
	else if (strcmp(token_copy, "STSW") == 0)
	{
		free(token_copy);
		return "E8";
	}
	else if (strcmp(token_copy, "STX") == 0)
	{
		free(token_copy);
		return "10";
	}
	else if (strcmp(token_copy, "SUB") == 0)
	{
		free(token_copy);
		return "1C";
	}
	else if (strcmp(token_copy, "COMP") == 0)
	{
		free(token_copy);
		return "28";
	}
	else if (strcmp(token_copy, "RD") == 0)
	{
		free(token_copy);
		return "D8";
	}
	else if (strcmp(token_copy, "TD") == 0)
	{
		free(token_copy);
		return "E0";
	}
	else if (strcmp(token_copy, "TIX") == 0)
	{
		free(token_copy);
		return "2C";
	}
	else if (strcmp(token_copy, "WD") == 0)
	{
		free(token_copy);
		return "DC";
	}
	free(token_copy);
	return "";
}

const char* travel_list(char token[])
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
	char plus_flag_bit[20];
	while (temp != NULL)
	{
		sprintf(plus_flag_bit, "%s,X", temp->symbol);
		if (token_copy != NULL && strcmp(temp->symbol, token_copy) == 0)
		{
			static char format_val[20];
			sprintf(format_val, "%04X", temp->value);
			free(token_copy);
			return format_val;
		}
		else if (token_copy != NULL && strcmp(plus_flag_bit, token_copy) == 0)
		{
			static char format_val[20];
			sprintf(format_val, "%04X", temp->value + 32768);
			free(token_copy);
			return format_val;
		}
		temp = temp->next;
	}
	free(token_copy);
	return "0000";
}