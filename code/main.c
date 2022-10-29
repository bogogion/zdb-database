#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

/*
	Written by Zane Maples as a side project.
	All code written here is subject to being stolen, rewritten, and modified for anyone that wants too.
*/

#define MAX_HEADER_TOKENS 22 /* Maximum tokens in the header */
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

data_file gen_db_file(char file_name[])
{
	/* Read data from .db file */

	FILE *file;
	file = fopen(file_name,"r");

	data_file db_file;

	char line[255];

	char temp_token[255];
	int cur_token = 0;

	int j = 0;
	while(fgets(line,255,file))
	{	
		db_file.vals++;
		for(int i = 0;i<strlen(line);i++)
		{
			if(line[i] == '{'){ continue; }
			if(line[i] == '}')
			{
				strcpy(db_file.file_paths[cur_token],temp_token);
				memset(temp_token,0,sizeof(temp_token));
				j = 0;
				break;
			}
			if(line[i] != ',')
			{
				temp_token[j] = line[i];
				j++;
			} 
			else
			{	
				strcpy(db_file.db_names[cur_token], temp_token);
				memset(temp_token,0,sizeof(temp_token));
				j = 0;
			}
		}
		cur_token++; /* Update only at end of loop */
		memset(line,0,sizeof(line)); /* Just 2 be safe!! */
	}
	fclose(file);
	return db_file;
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
			/* Add character to token */
			temp_token[j] = line[i];
			j = j + 1;
		}

	}
	return tdata;
}

int convert_val_usable(table_header theader,table_data *tdata, int no)
{
	/* no = val number

	   This function takes in the data and returns/assigns it based on the type described fo
	   the value in the header.

	   The data is assigned to the tdata struct passed as an argument

	   The values changed are last_read_{type}

	   See main.h for declaration of this struct.
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

char * gen_str_from_tdata(table_data tdata,table_header theader,char line[2560])
{ 	
	*line = '{'; /* Proper start of string */

	char id[20];
	snprintf(id,20,"%lld",tdata.id);
	unsigned int vals = theader.vals;

	char temp_token[255];

	/* Assign ID */
	strcat(line,id);
	strcat(line,",");
	for(int i = 0;i<vals;i++)
	{
		strcpy(temp_token,tdata.values[i]);
		strcat(line,temp_token);

		if(i+1 != vals){ strcat(line,","); }
		
		memset(temp_token,0,sizeof(temp_token));
	}
	strcat(line,"}");
	printf("%s\n",line);
	return line;
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

	/* Loop through lines in file to get required ones */
	
	while(fgets(line, 255, file))
	{
		if(counter == 0){strcpy(lines[0],line);}

		if(counter == search_id){strcpy(lines[1],line); break;}
		counter++;
	}
	
	/* Create a table header to parse data later on, the arguments for this should always
	   be the first line of the table.
	*/

	data_file db_file = gen_db_file("db/database.db");
	
	table_header theader = parse_table_header_line(lines[0],strlen(lines[0]));
	
	/* Create a table data struct, both table_header and table_data defined in main.h */

	table_data tdata = parse_table_data(lines[1],strlen(lines[1]));
	
	/* Convert values to usable types (type casting)*/
	char gen_line[2560];
	//gen_line[0] = '{';
	printf("NEW %s\n",gen_str_from_tdata(tdata,theader,gen_line));

	convert_val_usable(theader,&tdata,1);
	convert_val_usable(theader,&tdata,2);
	convert_val_usable(theader,&tdata,3);

	/* Print our data */
	
	printf("--- %s ---\n",theader.name);
	printf("Book Id: %i\n",tdata.id);
	printf("%s: %s\n",theader.val_names[0],tdata.last_read_string);
	printf("%s: $%.6g\n",theader.val_names[1],(tdata.last_read_double));
	printf("%s: %i\n",theader.val_names[2],(tdata.last_read_int));
	
	fclose(file);
	return 0;
}
