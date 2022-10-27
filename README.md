# ZDB - ZDataBase
### **CURRENT WIP**
Written solely by Zane Maples

### Current Features
- Full table features supporting up to 10 types of values, with sizes of 255 bytes (to be increased) with types of int, double, string, and long.
- ID search
- Complete type-casting abilities by using the table header
- Usable data parsed from a single string in the table file

### Planned Features (in order)
1. .db Files to manage .tb files (tables)
2. Write code to generate tables and entries
3. Indexing via primary keys
4. Convert functions to usable header files to import into other programs
5. Write a complete example program to manage databases
6. Search optimization and complete testing with current configurations (sizes)
7. Allow for dynamic allocation of elements (similar feature to SQL where you are able to create fields of a defined size)
8. Finishing polish and tweaks

### How it works
Code is located in the /code directory

Database files are located in /db

If you change the name/location of the table.tb file make sure to change it in main.c.
Example can be found in table.tb

To create a table create a header as the first line as so:

NAME#ID#VALNAM1#TYPE# - Supports up to 10 vals

Types supported:
- INT
- STR
- DBLE
- LONG

Then below the header you have your data. For example:

Test#ID#Text#STR#Number#INT

{1,TestText,5}

**IMPORTANT**

Name and Value Names can only go up to 20 characters currently. And each value in the table has to be less than *255* bytes long!

ID should increment by one based on the line! Do not skip increments or you will be unable to access the data.

For information on how to parse and use this data see the main.c main() where there is an example.

**HOW TO USE PROGRAM**
Once you have compiled the program via make do ./zdb -i #id. ie. ./zdb -i 4 for the 4th value.
