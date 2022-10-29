#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

/*
	Written by Zane Maples as a side project.
	All code written here is subject to being stolen, rewritten, and modified for anyone that wants too.
*/

int main(int argc, char *argv[])
{
	long search_id = 1; /* Default id */

	/* Simple code for arguments */
	if(argc == 3){if(strcmp("-i",argv[1])==0 && atol(argv[2]) > 0){ search_id = atol(argv[2]);}}
	
	FILE *file;
	char line[255];

	char lines[2][255];

	file = fopen("db/table.tb","r");
	int counter = 0;
	
	if(search_id > get_max_id("db/table.tb")){printf("ID not found.\n"); return -1;}

	/* Loop through lines in file to get required ones */
	
	while(fgets(line, 255, file))
	{
		if(counter == 0){strcpy(lines[0],line); }
		
		if(counter == search_id){strcpy(lines[1],line); break;}
		counter++;
	}
	
	/* Create a table header to parse data later on, the arguments for this should always
	   be the first line of the table.
	*/

	//data_file db_file = gen_db_file("db/database.db");
	table_header theader = parse_table_header_line(lines[0],strlen(lines[0]));
	/* Create a table data struct, both table_header and table_data defined in main.h */

	table_data tdata = parse_table_data(lines[1],strlen(lines[1]));
	
	/* Convert values to usable types (type casting)*/

	/*
	char *gen_line = (char*)malloc(2560 * sizeof(char));
	table_data test_data;
	
	strcpy(test_data.values[0],"Test");
	strcpy(test_data.values[1],"200");
	strcpy(test_data.values[2],"10.15");
	*/
	//file = fopen("db/COMP_1.tb","a");
	/*
	for(int i = 0;i<1000000;i++)
	{
		test_data.id = i+1;
		fputs(gen_str_from_tdata(test_data,theader,gen_line),file);
		memset(gen_line,0,sizeof(gen_line));
	} 
	*/
	
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
