#include <stddef.h>
#include <stdio.h>
typedef struct table_header
{
	char name[20];
	char val_names[10][15];
	char val_types[10][6];
	unsigned int vals : 4;

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

table_header parse_table_header_line(char line[],size_t size);
int convert_val_usable(table_header theader,table_data *tdata, int no);
table_data parse_table_data(char line[],size_t size);
long check_id(char line[],size_t size);
long get_max_id(char file_name[]);
