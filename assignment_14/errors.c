/*
 ============================= Maman 14 =====================================
 File Name   : errors.c
 Semester    : 2017b
 Description : This file contains the check_errors function that checks if the ERROR varible contains error number
				and prints the error on the screen.
 ============================================================================
*/

#include "errors.h"

/* returns true if the was an error and prints the error on the screen */
int check_errors()
{
	if(ERROR != NO_ERROR)
	{
		EC++;
		switch (ERROR)
		{
			case SYNTAX_ERROR:						
				printf("line %d: Syntax error. \n",LN);
				break ;
			case LABEL_INST_ERROR:
				printf("line %d: label cannot be an instruction.\n", LN);
				break ;
			case LABEL_DIR_ERROR:
				printf("line %d: label cannot be a directive. \n", LN);
				break;
			case DEF_LABEL_ERROR:
				printf("line %d: Previously defined label. \n",LN);
				break;
			case LONG_LABEL_ERROR:
				printf("line %d: label contains more than 30 characters. \n",LN);
				break;
			case DIR_ERROR:
				printf("line %d: Unrecognized directive. \n",LN);
				break;
			case INST_ERROR:
				printf("line %d: unrecognized insruction. \n",LN);
				break;
			case FCHAR_LABEL_ERROR:
				printf("line %d: The first character of the label must be a letter. \n", LN);
				break;
			case LONG_LINE_ERROR:
				printf("line %d: To many characters for one line. The number of characters per line must be less than 80.\n",LN	);
				break;
			case BAD_ARG_ERROR:
				printf("line %d: bad argument[s]\n",LN);
				break;
			case ONLY_LABEL_ERROR:
				printf("Line %d: line should contain more than a label\n",LN);
				break;
			case ADD_ERROR:
				printf("Line %d: Illegal addressing, check operands. \n",LN);
				break;
			case LABEL_REG_ERROR: 
				printf("Line %d: label cannot be a register \n",LN);
				break;
			case NDEF_LABEL_ERROR:
				printf("Line %d: label was not defined in the file \n",LN);
				break;
			case NO_ARG_ERROR:
				printf("Line %d: Error: expecting argument...\n",LN);
				break;
		}
		ERROR = NO_ERROR;
		return YES;
	}
	else 
		return NO;
}