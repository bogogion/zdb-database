#include <stddef.h>
#include <stdio.h>
typedef struct table_header
{
	char name[20];
	char val_names[10][15];
	char val_types[10][6];
	unsigned int vals : 5;

} table_header;
/* last_read_int long to support larger numbers */
typedef struct table_data
{
	long id;
	char values[10][255];
	long last_read_int;
	double last_read_double;
	char last_read_string[255];
} table_data;
typedef struct db_file
{	
	char db_names[20][20]; /* See table_header for name size. */
	char file_paths[20][50];
	unsigned int vals : 5;
} data_file;

table_header parse_table_header_line(char line[],size_t size);
int convert_val_usable(table_header theader,table_data *tdata, int no);
table_data parse_table_data(char line[],size_t size);
long get_max_id(char file_name[]);
data_file gen_db_file(char file_name[]);
char * gen_str_from_tdata(table_data tdata,table_header theader,char line[2560]);
