/*
 ============================= Maman 14 =====================================
 File Name   : read_line.c
 Semester    : 2017b
 Description : This file contains functions for scanning the lines of the file, reading and decoding each word in the line.
 ============================================================================
*/

#include "read_line.h"
/* reads each line and check its validation */
void read_line(char *line)
{	
	/* if the line is empty line or comment ignore it */
	if(ignore_line(line) == YES)
		return;
	
	/* remove the spaces before first letter */
	line = skip_space(line);

	/* if the line is too long */
	if(strlen(line) > MAX_LINE_LEN-1)
	{
		ERROR = LONG_LINE_ERROR;
		return;
	}
	
	/* if the first word is a label and it is vaild then put it in symbol table and go to next word */
	if(is_label(line))
		line = next_word(line);
	
	if(check_errors())
		return;
		
	if(!isalpha(*line) && *line != '.')
	{   
		ERROR = SYNTAX_ERROR;
		return;
	}
	
	/* if the word is directive sentence it check its validation then put it in data table */
	if(*line == '.')
	{
		int dirtype = is_dir(line+1);
		
		if(dirtype != NON)
		{
			line = next_word(line);
			if(check_dir(line, dirtype))
				return;
		}
	}
	
	if(check_errors())
		return;
	
	/* if the word is instruction sentence it checks its validation */
	if(is_inst(line) >= MOV_INST)
	{
		check_inst(line);
		check_errors();
	}
		
	/* if its not directive or instruction sentence */
	else if(ERROR == NO_ERROR)
	{
		ERROR = INST_ERROR;
		return;
	}
}

/* returns true if the line is empty or a comment */
int ignore_line(char *line)
{
	if(line == NULL)
		return YES;
	if(*line==';')
		return YES;
	while(isspace(*line))
	{
		line++;
		if(*line==';')
			return YES;
	}
	if(*line=='\0')
		return  YES;
	else 
		return NO;
}

/* skipping the space chars before the word */
char *skip_space(char *line)
{
	if(line == NULL)
		return NULL;
	
	while(isspace(*line))
		line++;
	
	if (*line == '\0')
		return NULL;
	
	return line;
}

/* jumps to the next word */
char *next_word(char *line)
{
	if(line == NULL)
		return NULL;
	
	while(!isspace(*line) && *line != '\0')
	{
		line++;
	}
	
	line = skip_space(line);
	
	return line;
}

/* jumps to the word after the comma */
char *to_comma(char *line)
{
	if(line == NULL)
		return NULL;
	
	while(*line != ',' && !isspace(*line))
	{
		if(*line == '\0')
			return NULL;
		
		line++;
	}
	
	line++;
	line = skip_space(line);
	return line;
}