/*
 ============================= Maman 14 =====================================
 File Name   : first_pass.c
 Semester    : 2017b
 Description : This file contains the first_pass function. this funcion runs the first scan on the .as file,
 the purpose of the first pass is to check the validation of each line, calculate the file's memmory consumption and
 insert each symbol to a symbol table.
			 
 ============================================================================
*/

#include "first_pass.h"

/* the first pass function */
void first_pass(char *file)
{
	FILE *fp ;
	char file_name[FILE_NAME_LEN];
	char line[BUFF_SIZE];
	
	/* adds .as ending to the name of the file and opens it */
	strcpy(file_name,file);
	strcat(file_name,".as");	
	fp = fopen(file_name,"r");
	
	/* initializing the global variables */
	IC = IC_START;
	DC = 0;
	EC = NO_ERROR;
	LN = 0;
	ERROR = NO_ERROR;
	
	/* checks if the given file is acceptable */
	if(!fp)
	{
		printf("\n\n \"%s\" \n does not exist or no access.\n\n",file_name);
		return;
	}

	/* checks each line and count the IC */
	while(fgets(line,BUFF_SIZE,fp) != NULL)
	{
		LN++;
		read_line(line);
		check_errors();
	}

	/* updating the DC counting according the IC */
	update_DC();
	check_errors();
		
	/* prints the total amount of errors */
	if(EC > NO_ERROR)
	{
		printf("\n%s contains %d errors.\n",file_name,EC);
		return;
	}
	
	/* if there are no errors continue to second pass */
	else
	{
		rewind(fp);
		second_pass(fp, file);
		return;
	}
}