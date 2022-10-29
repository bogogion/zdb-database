#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../main.h"

/*
	Written by Zane Maples as a side project.
	All code written here is subject to being stolen, rewritten, and modified for anyone that wants too.

	NOTE: For this particular piece of code, their is NO makefile for the time being.
	To compile, just use your standard compiler with code/main.h included.

*/

int main(int argc, char *argv[])
{
	
	FILE *file;
	char line[255];

	char lines[1][255];
	
	/* IMPORTANT! In order to get this code to run, make sure to copy the file named
	"COMP_1_HEADER" to another file, as this file is the header that causes the code to work
	Then replace all references to filenames as the new file you created.

	ie. cp COMP_1_HEADER -> COMP_1.tb

	This code also ONLY generates the file, in order to view it, change files references in
	code/main.c.

	*/

	file = fopen("../../db/COMP_1.tb","a+");
	int counter = 0;

	/* Loop through lines in file to get required ones */
	
	while(fgets(line, 255, file))
	{
		if(counter == 0){strcpy(lines[0],line); break; }
	}
	
	table_header theader = parse_table_header_line(lines[0],strlen(lines[0]));
	/* Create a table data struct, both table_header and table_data defined in main.h */

	char *gen_line = (char*)malloc(2560 * sizeof(char));
	table_data test_data;

	/* You can replace these values with custom values,
	just make sure that they align with the table header. */

	strcpy(test_data.values[0],"Test");
	strcpy(test_data.values[1],"200");
	strcpy(test_data.values[2],"10.15");

	int amount = 1000; /* Amount of entries */
	for(int i = 0;i<amount;i++)
	{
		test_data.id = i+1;
		fputs(gen_str_from_tdata(test_data,theader,gen_line),file);
		memset(gen_line,0,sizeof(gen_line));
	} 
	printf("SUCCESS! Check file.\n");
	fclose(file);
	return 0;
}
