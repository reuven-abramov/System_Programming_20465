/*
 ============================= Maman 14 =====================================
 File Name   : main.c
 Semester    : 2017b
 Description : This file contains the main function of the project that checks if there is a file to assemble and assemble each given file.
 ============================================================================
*/

#include "main.h"

/* the main function of the project */
int main(int argc, char *argv[])
{
	int i;
	
	/* checks if there is file to compile */
	if(argc < 2)
	{
		printf("\nno file to assemble\n\n");
		exit(0);
	}
	
	/* compile each file */
	for(i=1;i<argc;i++)
	{
		first_pass(argv[i]);
		free_tables();
	}
	
	return 0; 
}