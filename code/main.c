#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

#define MAX_LINE_LENGTH 255 /* Macro for how long each line in a file can be */

#define MAX_HEADER_TOKENS 22 /* Maximum tokens in the header e*/
#define MAX_TOKEN_LENGTH 20 /* Largest token is NAME */

/* See "main.h" for declaration of structs */
table_header parse_table_header_line(char line[],size_t size)
{
	/* Generate table_header struct */
	struct table_header theader;
	/* Max of 22 tokens, 2 Tokens for NAME + ID, rest for actual VALs */
	char tokens[MAX_HEADER_TOKENS][MAX_TOKEN_LENGTH+1];
	char temp_token[MAX_TOKEN_LENGTH+1];
	
	int token_count = 0;
	int cur_t_counter = 0;
	int t_counter = 0;

	/* Tokenize current string, token seperators are # */
	for(int i = 0;i < size;i++)
	{
		if(line[i] != '#')
		{
			temp_token[t_counter] =	line[i];
			t_counter++;
		} else {
			strcpy(tokens[cur_t_counter],temp_token);

			cur_t_counter++;
			t_counter = 0;
			token_count++;

			memset(temp_token,0,sizeof(temp_token));
		}
	}

	/* First element in tokens should always be NAME, the second element ([1]) is ID, and is disregarded
	   in header processing (viewed later in actual data analysis.)
	*/
	strcpy(theader.name, tokens[0]);

	/* Minus two tokens for NAME and ID*/
	int cur_v_token = 0; /* VAL Name*/
	int cur_type_token = 0; /* VAL Type*/

	/* Filter through tokens and assign them to struct table_header, vals is amount of vals, 4 bit int.*/
	for(int i = 1;i<token_count-1;i++)
	{
		if(i % 2 != 0 )
		{ strcpy(theader.val_names[cur_v_token],tokens[i+1]); cur_v_token++; theader.vals++; }
	  	else
		{ strcpy(theader.val_types[cur_type_token],tokens[i+1]); cur_type_token++; }
	}
	return theader;
}

table_data parse_table_data(char line[],size_t size)
{
	table_data tdata; /* Only one of these should be used if possible */
	
	int cur_token = 0; /* Temporary storage of elements */
	char temp_token[255];
	long id;
	int j = 0;
	for(int i = 0;i<size;i++)
	{	
		if(line[i]=='{')
		{	
			continue;
		}
		if(line[i]=='}')
		{
			strcpy(tdata.values[cur_token-1],temp_token);
			memset(temp_token,0,sizeof(temp_token));
			break; /* End indentifier */
		}
		
		/* Serious Parsing */
		if(line[i] == ',')
		{
			/* Reset */
			j = 0;
			if(cur_token == 0)
			{
				id = atol(temp_token);
				tdata.id = id;
				cur_token++;
				memset(temp_token,0,sizeof(temp_token));
			}
			else
			{
				strcpy(tdata.values[cur_token-1],temp_token);
				cur_token++;
				memset(temp_token,0,sizeof(temp_token));
			}
		} else 
		{	
			temp_token[j] = line[i];
			j = j + 1;
		}

	}
	return tdata;
}

long check_id(char line[],size_t size)
{	
	/* Check the first element, which should always be ID and compare*/
	long id;
	char token[size];
	for(int i = 1;i<size;i++)
	{
		if(line[i] != ',')
		{
			token[i-1] = line[i];
		} else {
			return atol(token);
		}
	}
}

int convert_val_usable(table_header theader,table_data *tdata, int no)
{
	/* no = val number

	   This function takes in the data and returns/assigns it based on the type described fo
	   the value in the header.

	*/

	/* Check if no out of bound*/
	if(theader.vals < no){ return -1; }
	
	if(strcmp("INT",theader.val_types[no-1]) == 0)
	{tdata->last_read_int = atoi(tdata->values[no-1]); return 1;}
	
	if(strcmp("STR",theader.val_types[no-1]) == 0)
	{strcpy(tdata->last_read_string,tdata->values[no-1]); return 2;}
	
	if(strcmp("DBLE",theader.val_types[no-1]) == 0)
	{tdata->last_read_double = atof(tdata->values[no-1]); return 3;}

	if(strcmp("LONG",theader.val_types[no-1]) == 0)
	{tdata->last_read_int = atol(tdata->values[no-1]); return 4;}

	/* If type checking fails return */
	return -1;
}

long get_max_id(char file_name[])
{	
	FILE *file;
	file = fopen(file_name,"r");
	long ids = 1;
	char ch;
	while(!feof(file))
	{
		ch = fgetc(file);
		if(ch == '\n'){ ids++; }
	}
	fclose(file);
	return ids-2; /* Top line to be ignored*/
}

int main(int argc, char *argv[])
{
	long search_id = 1; /* Default id */
	/* Simple code for arguments */
	if(argc == 3){if(strcmp("-i",argv[1])==0 && atol(argv[2]) > 0){ search_id = atol(argv[2]);}}
	
	FILE *file;
	char line[255];

	char lines[2][255];

	file = fopen("db/table.tb","r");
	if(file == NULL){ printf("UNABLE TO OPEN FILE"); return -1; }
	
	int counter = 0;
	
	if(search_id > get_max_id("db/table.tb")){printf("ID not found.\n"); return -1;}
	
	/* Flush file buffer */
	
	while(fgets(line, 255, file))
	{
		if(counter == 0){strcpy(lines[0],line);}
		if(check_id(line,strlen(line)) == search_id){strcpy(lines[1],line); break;}
		counter++;
		//for(int i = 0;i<MAX_LINE_LENGTH;i++){if(line[i] == '#'){line[i]=' ';}}
		//printf(line);
	}

	table_header theader = parse_table_header_line(lines[0],strlen(lines[0]));
	
	/* Test */

	table_data tdata = parse_table_data(lines[1],strlen(lines[1]));

	convert_val_usable(theader,&tdata,1);
	convert_val_usable(theader,&tdata,2);
	convert_val_usable(theader,&tdata,3);

	//printf("%s\n",lines[1]);
	printf("--- %s ---\n",theader.name);
	printf("Book Id: %i\n",tdata.id);
	printf("%s: %s\n",theader.val_names[0],tdata.last_read_string);
	printf("%s: $%.6g\n",theader.val_names[1],(tdata.last_read_double));
	printf("%s: %i\n",theader.val_names[2],(tdata.last_read_int));
	fclose(file);
	return 0;
}