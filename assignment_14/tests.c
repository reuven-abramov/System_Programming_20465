/*
 ============================= Maman 14 =====================================
 File Name   : tests.c
 Semester    : 2017b
 Description : This file contains functions that tests the validation of the 
			   file.
 ============================================================================
*/

#include "tests.h"

keywords instruction[] =
{
  {"mov"},{"cmp"},{"add"},{"sub"},{"not"},{"clr"},{"lea"},{"inc"},
  {"dec"},{"jmp"},{"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"}
};

keywords directive[] = 
{
  {"data"},{"string"},{"mat"},
  {"entry"},{"extern"}
};

/* checks if the word is a valid label and put it in symbol table */
int is_label(char *line)
{
	char label[MAX_LABEL_LEN];
	char *p = line;
	
	int  i = 0, c = 0, address = 0, ext_flag = NO, inst_flag = NO;
	
	while (*p != '\0') 
	{
		/* if it is label */
		if (*p == ':') 
		{
			/* max length of a label is 30 characters */
			if (i >= MAX_LABEL_LEN) 
			{
				ERROR = LONG_LABEL_ERROR;
				return NO;
			}
			
			p = line;
			
			/* first character of label have to be letter */
			if (!isalpha(*p))
			{
				ERROR = FCHAR_LABEL_ERROR;
				return NO;
			}
			
			/* label characters can be only letters or digits */
			for(c=0;c<i;c++)
			{
				label[c] = *p;
				
				/* label characters can be only letters or digits */
				if(!isdigit(label[c]) && !isalpha(label[c]))
				{
					ERROR = SYNTAX_ERROR;
					return NO;
				}
				
				p++;
			}
			
			label[c] = '\0';
			
			/* check if the label is one of the saved words */
			if(is_inst(label) >= MOV_INST)
			{
				ERROR = LABEL_INST_ERROR;
				return NO;
			}
			
			if(strlen(label) == REG_LEN && label[0] == 'r' && label[1] >= '0' && label[1] <= '7')
			{
				ERROR = LABEL_REG_ERROR;
				return NO;
			}

			/* after a label it has to be somthing */
			if(next_word(line) == NULL)
			{
				ERROR = ONLY_LABEL_ERROR;
				return NO;
			}
			
			/* if the label points to directive then the symbol addres is the DC */
			if(*(next_word(line)) == '.')
			{
				if(is_dir(next_word(line)+1) >= ENTRY_DIR)
					return YES;
				
				else
					address = DC;
				
				check_errors();
			}
				
			/* if the label points to instruction then the symbol addres is the IC */
			else if(is_inst(next_word(line)) >= MOV_INST)
			{
				inst_flag = YES;
				address = IC;
			}
			
			/* label cannot be declared twice */
			if(search_sym(label))
			{
				ERROR = DEF_LABEL_ERROR;
				return NO;
			}
			
			to_symbol(label, address, ext_flag, inst_flag);
			return YES;
		}
		else
		{
			i++;
			p++;
		}
	}
	return NO;
}

/* checks if the word is a directive and returns it's number */
int is_dir(char *line)
{
	int i, c = 0; 
	char *p = line;
	char dir[MAX_DIRECTIVE_LEN];

	while(!isspace(*p) && *p != '\0')
	{
		if(isalpha(*p))
			dir[c]=*p;
		
		else
		{
			ERROR = DIR_ERROR;
			return NON;
		}
		
		c++;
		p++;
	}
	
	dir[c] = '\0';
	
	for(i=0;i<DIR_SIZE;i++)
	{
		if (!strcmp(directive[i].word, dir))
		{
			if(next_word(line) == NULL)
			{
				ERROR = NO_ARG_ERROR;
				return NON;
			}
			
			return i;
		}
	}
	
	ERROR = DIR_ERROR;
	return NON;
}

/* checks if the word is a instruction and returns it's number */
int is_inst(char *line)
{
	int i,c = 0;
	char *p = line;
	char inst[MAX_INSTRACTION_LEN];
	
	while(!isspace(*p) && *p != '\0')
	{
		if(isalpha(*p) && c < MAX_INSTRACTION_LEN)
			inst[c]=*p;
		else
			return NON;
		
		c++;
		p++;
	}
	
	inst[c] ='\0';
	
	for(i=0;i<=INST_SIZE;i++)
		if (strcmp(instruction[i].word, inst) == NO)
			return i ;
	
	return NON ;
}

/* checks the validation of the directive sentence */
int check_dir(char *line, int dirtype)
{	
	switch(dirtype)
	{
		/* if the directive is immidiate number */
		case DATA_DIR :
			if(line != NULL)
			{
				char data[MAX_INTEGER_LEN];
				int integer, i;
				
				while(YES)
				{
					if(*line == '-' || *line == '+' || isdigit(*line))
					{
						data[0] = *line;
						line++;
						
						for(i=1;i<MAX_INTEGER_LEN && isdigit(*line);i++)
						{
							data[i] = *line;
							line++;
						}
						
						data[i] = '\0';
						integer = atoi(data);
						
						/* checks if the  integer fits 10 bits */
						if(integer >= MIN_DATA_INT && integer <= MAX_DATA_INT)
							to_data(integer);
						
						else
						{
							ERROR = BAD_ARG_ERROR;
							return NO;
						}
						
						line = skip_space(line);
						
						if(line == NULL)
							return YES;
						
						if(*line != ',')
						{
							ERROR = SYNTAX_ERROR;
							return NO;
						}
						
						line = skip_space(line+1);
						
						if(line == NULL)
						{
							ERROR = SYNTAX_ERROR;
							return NO;
						}
					}
					
					else 
					{
						ERROR = SYNTAX_ERROR;
						return NO;
					}
				}
			}
			
			else
				return YES;
			
			break;

		/* if the directive is a string */
		case STRING_DIR :
			if(line != NULL)
			{
				int ch;

				if(*line == '"') 
				{
					line++;
					while(*line != '"')
					{
						if(*line == '\0')
						{
							ERROR = SYNTAX_ERROR;
							return NO;
						}
						ch = (int)*line;
						to_data(ch);
						line++;
					}
					
					ch = 0;
					to_data(ch);
					
					if(skip_space(line+1) == NULL)
						return YES;
					
					else
					{
						ERROR = SYNTAX_ERROR;
						return NO;
					}
				}
				
				else
				{
					ERROR = SYNTAX_ERROR;
					return NO;
				}
			}
			
			else 
			{
				ERROR = SYNTAX_ERROR;
				return NO;
			}
			
			break;
			
		/* if the directive is a matrix */
		case MAT_DIR :
			if(line != NULL)
			{
				char data[MAX_INTEGER_LEN];
				int datasize, integer, i;
				
				if(*line == '[')
				{
					line++;
					
					if(!isdigit(*line))
					{
						ERROR = BAD_ARG_ERROR;
						return NO;
					}
					
					for(i=0;*line != ']';i++)
					{
						if(isdigit(*line))
							data[i] = *line;
						
						else 
						{
							ERROR = SYNTAX_ERROR;
							return NO;
						}
						
						line++;
					}
					
					data[i] = '\0';
					datasize = atoi(data);
					
					if(*line == ']' && *(line+1) == '[')
					{
						line = line+2;
						
						if(!isdigit(*line))
						{
							ERROR = BAD_ARG_ERROR;
							return NO;
						}
					
						for(i=0;*line != ']';i++)
						{
							if(isdigit(*line))
								data[i] = *line;
							
							else 
							{
								ERROR = SYNTAX_ERROR;
								return NO;
							}
							
							line++;
						}
					
						data[i] = '\0';
						datasize *= atoi(data);
					}
					
					else
					{
						ERROR = SYNTAX_ERROR;
						return NO;
					}
										
					line = skip_space(line+1);
					
					for(i=0;i<datasize;i++)
					{
						if(line != NULL)
						{
							int j;
							
							if(*line == '-' || *line == '+' || isdigit(*line))
							{
								data[0] = *line;
								line++;
								
								for(j=1;j<MAX_INTEGER_LEN && isdigit(*line);j++)
								{
									data[j] = *line;
									line++;
								}
								
								data[j] = '\0';
								integer = atoi(data);
								
								if(integer >= MIN_DATA_INT && integer <= MAX_DATA_INT)
									to_data(integer);
								
								else
								{
									ERROR = BAD_ARG_ERROR;
									return NO;
								}
								
								line = skip_space(line);

								if(line == NULL)
								{
									
								}
								
								else if(*line == ',')
								{
									line = skip_space(line+1);
								}
							}
							
							else 
							{
								ERROR = SYNTAX_ERROR;
								return NO;
							}
						}
						
						else
							to_data(0);
					}
					if(line != NULL)
					{
						ERROR = BAD_ARG_ERROR;
						return NO;
					}
					
					else
						return YES;
					
				}
				
				else 
				{
					ERROR = SYNTAX_ERROR;
					return NO;
				}
			}
			
			else 
			{
				ERROR = SYNTAX_ERROR;
				return NO;
			}
				
			break;
			
		/* if the directive is entry label */
		case ENTRY_DIR :
			return YES;
			break;
		
		/* if the directive is external label */
		case EXTERN_DIR :
			if(line != NULL)
			{
				char label[MAX_LABEL_LEN];
				int  i=0;
				
				if (!isalpha(*line))
				{
					ERROR = FCHAR_LABEL_ERROR;
					return NO;
				}
				
				for(i=0;!isspace(*line);i++)
				{
					label[i] = *line;
					
					if(!isdigit(label[i]) && !isalpha(label[i]))
					{
						ERROR = SYNTAX_ERROR;
						return NO;
					}
					
					line++;
				}
				
				label[i] = '\0';
				
				if(skip_space(line) == NULL)
				{
					if(search_sym(label) == NULL)
					{
						to_symbol(label, NO, YES, NO);
						return YES;
					}
					
					else
					{
						ERROR = DEF_LABEL_ERROR;
						return NO;
					}
				}
				
				else
				{
					ERROR = SYNTAX_ERROR;
					return NO;
				}
			}
			
			else 
			{
				ERROR = SYNTAX_ERROR;
				return NO;
			}
			
			break;
	}
	
	return NO;
}

/* checks the validation of the instruction sentence */
int check_inst(char *line)
{
	int opcode = is_inst(line);
	int addressing1, addressing2;
	line = next_word(line);
	
	/* if the instruction doesn't need operands */
	if(opcode > JSR_INST)
	{
		if(line == NULL)
		{
			IC++;
			return YES;
		}
		
		else
		{
			ERROR = SYNTAX_ERROR;
			return NO;
		}
	}
	
	if(line == NULL)
	{
		ERROR = BAD_ARG_ERROR;
		return NO;
	}
	
	addressing1 = check_addressing(line);
	
	if(ERROR != NO_ERROR)
		return NO;
	
	line = to_comma(line);

	/* if first operand is immidiate number */
	if(addressing1 == IMM_ADDRESS)
	{
		if(opcode > SUB_INST && opcode != PRN_INST)
		{
			ERROR = ADD_ERROR;
			return NO;
		}
		
		if(opcode == PRN_INST)
		{
			if(line == NULL)
			{	
				IC += 2;
				return YES;
			}
			
			else
			{
				ERROR = ADD_ERROR;
				return NO;
			}			
		}
		
		IC++;
	}
	
	/* if first operand isn't immidiate number */
	else if(addressing1 > IMM_ADDRESS)
	{
		if(opcode == LEA_INST && addressing1 == SUB_INST)
		{
			ERROR = ADD_ERROR;
			return NO;
		}
		
		if(opcode > SUB_INST && opcode != LEA_INST)
		{
			if(line == NULL)
			{
				IC++;
				
				if(addressing1 == MAT_ADDRESS)
					IC += 2;
				else
					IC++;
					
				return YES;
			}
			
			else
			{
				ERROR = ADD_ERROR;
				return NO;
			}	
		}
		
		if(addressing1 == MAT_ADDRESS)
			IC += 2;
		else
			IC++;
	}
	
	if(line == NULL)
	{
		ERROR = SYNTAX_ERROR;
		return NO;
	}
	
	addressing2 = check_addressing(line);
	
	if(ERROR != NO_ERROR)
		return NO;
	
	if(next_word(line) == NULL && to_comma(line) == NULL)
	{
		/* if second operand is immidiate number */
		if(addressing2 == IMM_ADDRESS)
		{
			if(opcode == CMP_INST || opcode == PRN_INST)
			{
				IC += 2;
				return YES;
			}
			
			else 
			{
				ERROR = ADD_ERROR;
				return NO;
			}
		}
		
		/* if second operand isn't immidiate number */
		if(addressing2 > IMM_ADDRESS)
		{
			IC++;
			
			if(addressing2 == MAT_ADDRESS)
				IC += 2;
			else if((addressing2 == REG_ADDRESS && addressing1 != REG_ADDRESS) || addressing2 == LABEL_ADDRESS)
				IC++;
			
			return YES;
		}
	}
	
	else
	{
		ERROR = SYNTAX_ERROR;
		return NO;
	}
	
	return NO;
}

/* checks operand addressing */
int check_addressing(char *line)
{
	char operand[MAX_OPERAND_LEN];
	int i;
	
	for(i=0;!isspace(*line) && *line != '\0' && *line != ',';i++)
	{
		operand[i] = *line;
		line++;
	}
	
	operand[i] = '\0';
	
	if(operand[0] == '#')
	{
		if(operand[1] == '-' || operand[1] == '+' || isdigit(operand[1]))
		{
			if((operand[1] == '-' || operand[1] == '+') && !isdigit(operand[2]))
			{
				ERROR = BAD_ARG_ERROR;
				return NON;
			}
			
			for(i=2;operand[i] != '\0';i++)
			{
				if(!isdigit(operand[i]))
				{
					ERROR = ADD_ERROR;
					return NON;
				}
			}
			
			i = atoi(operand+1);
			
			if(i > MAX_IMM_INT || i < MIN_IMM_INT)
			{
				ERROR = BAD_ARG_ERROR;
				return NON;
			}
			
			else
				return IMM_ADDRESS;
		}
		
		else
		{
			ERROR = SYNTAX_ERROR;
			return NON;
		}
	}
	
	if(strlen(operand) == REG_LEN && operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7')
		return REG_ADDRESS;
	
	if(isalpha(operand[0]))
	{
		for(i=1;i<strlen(operand);i++)
		{
			if(operand[i] == '[')
			{
				i++;
				if(operand[i] == 'r')
				{
					i++;
					if(operand[i] >= '0' && operand[i] <= '7')
					{
						i++;
						if(operand[i] == ']')
						{
							i++;
							if(operand[i] == '[')
							{
								i++;
								if(operand[i] == 'r')
								{
									i++;
									if(operand[i] >= '0' && operand[i] <= '7')
									{
										i++;
										if(operand[i] == ']')
										{
											i++;
											if(operand[i] == '\0')
												return MAT_ADDRESS;
										}
									}
								}
							}
						}
					}
				}
				
				ERROR = SYNTAX_ERROR;
				return NON;
			}

			if(!isalpha(operand[i]) && !isdigit(operand[i]))
			{
				ERROR = BAD_ARG_ERROR;
				return NON;
			}
		}
		
		return LABEL_ADDRESS;
	}
	
	ERROR = ADD_ERROR;
	return NON;
}