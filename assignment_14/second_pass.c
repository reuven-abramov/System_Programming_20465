/*
 ============================= Maman 14 =====================================
 File Name   : second_pass.c
 Semester    : 2017b
 Description : the second pass purpose is to encode each instruction and 
			   it's addresses, convert each instruction to strange four base 
			   counting and print it to the correct files.
 ============================================================================
*/

#include "second_pass.h"

/* the second pass of the project */
void second_pass(FILE *fp, char *file)
{
	char ob_name[FILE_NAME_LEN], ent_name[FILE_NAME_LEN], ext_name[FILE_NAME_LEN];
	FILE *ob_file;
	FILE *ent_file;
	FILE *ext_file;
	char line[BUFF_SIZE];
	
	/* opens 3 files, object file, entry file and extern file */
	strcpy(ob_name,file);
	strcpy(ent_name,file);
	strcpy(ext_name,file);
	strcat(ob_name,".ob");
	strcat(ent_name,".ent");
	strcat(ext_name,".ext");
	ob_file = fopen(ob_name, "w");
	ent_file = fopen(ent_name, "w");
	ext_file = fopen(ext_name, "w");
	
	/* initializing the global variables */
	IC = IC_START;
	EC = NO_ERROR;
	LN = NO;
	ERROR = NO_ERROR;
	
	/* reads each line of the file */
	while(fgets(line,BUFF_SIZE,fp) != NULL)
	{
		LN++;
		read_line2(line, ob_file, ent_file, ext_file);
		check_errors();
	}
	
	/* prints the incoded data to the files */
	print_to_files(ob_file, ent_file, ext_file);
	
	/* if one of the files is empty its delete it */
	if(ftell(ob_file) == NO)
	{
		fclose(ob_file);
		remove(ob_name);
	}

	if(ftell(ent_file) == NO)
	{	
		fclose(ent_file);
		remove(ent_name);
	}
	
	if(ftell(ext_file) == NO)
	{
		fclose(ext_file);
		remove(ext_name);
	}
	
	/* closes the files */ 
	fclose(fp);
	fclose(ob_file);
	fclose(ent_file);
	fclose(ext_file);
	
	/* check if there was errors */
	if(EC > NO)
	{
		printf("\n%s contains %d errors.\n",file ,EC);
		remove(ob_name);
		remove(ent_name);
		remove(ext_name);
		return;
	}
	
	
}

/* the function reads each line, checks its validation and incoding it */
void read_line2(char *line, FILE *ob_file, FILE *ent_file, FILE *ext_file)
{
	char *p = line;
	symbol_node *symbol;
	
	/* if the line is empty line or comment ignore it */
	if(ignore_line(line) == YES)
		return;
	
	line = skip_space(line);

	/* checks if there is a label */
	while(*p != '\0')
	{
		if(*p == ':')
			line = next_word(line);
		
		p++;
	}
	
	/* checks if it is directive sentence */
	if(*line == '.')
	{
		int dirtype = is_dir(line+1);
		
		/* if the sentence is an entry declaration then put it in table */
		if(dirtype == ENTRY_DIR)
		{
			char label[MAX_LABEL_LEN];
			int  i;
			
			line = next_word(line);
			
			/* copy the entry label to char array */
			for(i=0;!isspace(*line);i++)
			{
				label[i] = *line;
				line++;
			}
			
			label[i] = '\0';
			symbol = search_sym(label);
			
			/* if there is no such symbol */
			if(symbol == NULL)
			{
				ERROR = NDEF_LABEL_ERROR;
				return;
			}
			
			else
			{
				to_ent_ext(label, symbol->address, symbol->ext_flag);
				return;
			}
		}
		
		else
			return;
	}
	
	/* checks if it is instruction sentence */
	if(is_inst(line) >= MOV_INST)
	{
		int opcode = is_inst(line);
		int addressing1 = 0, addressing2 = 0;
		int i, data = 0, reg_src = 0, reg_dst = 0;
		char label[MAX_LABEL_LEN];
		
		line = next_word(line);
		
		/* if the instruction doesn't need operands */
		if(opcode > JSR_INST)
		{
			to_inst(INST_TYPE, opcode, addressing1, addressing2, data, ABSOLUTE);
			IC++;
			return;
		}
		
		/* if the instruction needs two operands */
		else if(opcode < NOT_INST || opcode == LEA_INST)
		{
			addressing1 = check_addressing(line);
			addressing2 = check_addressing(to_comma(line));
			to_inst(INST_TYPE, opcode, addressing1, addressing2, data, ABSOLUTE);
			IC++;
			
			/* if first operand is immidiate number */
			if(addressing1 == IMM_ADDRESS)
			{
				line++;
				
				for(i=0;*line!=',';i++)
				{
					label[i] = *line;
					line++;
				}
				
				label[i] = '\0';
				data = atoi(label);
				to_inst(DATA_TYPE, opcode, addressing1, addressing2, data, ABSOLUTE);
				IC++;
			}
			
			/* if first operand is a label */
			if(addressing1 == LABEL_ADDRESS)
			{
				for(i=0;*line!=',';i++)
				{
					label[i] = *line;
					line++;
				}
				
				label[i] = '\0';
				symbol = search_sym(label);
				
				if(symbol == NULL)
				{
					ERROR = NDEF_LABEL_ERROR;
					return;
				}
				
				/* if the label is external */
				else if(symbol->ext_flag == YES)
				{
					to_ent_ext(label, IC, symbol->ext_flag);
					to_inst(LABEL_TYPE, opcode, addressing1, addressing2, NO, EXTERNAL);
					IC++;
				}
				
				else
				{
					to_inst(LABEL_TYPE, opcode, addressing1, addressing2, symbol->address, RELOCATABLE);
					IC++;
				}
			}
			
			/* if first operand is matrix */
			if(addressing1 == MAT_ADDRESS)
			{
				for(i=0;*line!='[';i++)
				{
					label[i] = *line;
					line++;
				}
				
				label[i] = '\0';
				symbol = search_sym(label);
				
				if(symbol == NULL)
				{
					ERROR = NDEF_LABEL_ERROR;
					return;
				}
				
				/* if the matrix is external */
				else if(symbol->ext_flag == YES)
				{
					to_ent_ext(label, IC, symbol->ext_flag);
					to_inst(LABEL_TYPE, opcode, addressing1, addressing2, NO, EXTERNAL);
					IC++;
				}
				
				else
				{
					to_inst(LABEL_TYPE, opcode, addressing1, addressing2, symbol->address, RELOCATABLE);
					IC++;
				}
				
				line += 2;
				reg_src = *line - '0';
				line += 4;
				reg_dst = *line - '0';
				to_inst(REG_TYPE, opcode, reg_src, reg_dst, data, ABSOLUTE);
				IC++;
			}
			
			/* if first operand is  a register */
			if(addressing1 == REG_ADDRESS)
			{
				reg_src = *(line+1) - '0';
				if(addressing2 != REG_ADDRESS)
					to_inst(REG_TYPE, opcode, reg_src, reg_dst, data, ABSOLUTE);
				IC++;
			}
			
			line = to_comma(line);
		}
		
		/* if the instruction needs one operands */
		else
		{
			addressing2 = check_addressing(line);
			to_inst(INST_TYPE, opcode, addressing1, addressing2, data, ABSOLUTE);
			IC++;
		}
		
		/* if second operand is immidiate number */
		if(addressing2 == IMM_ADDRESS)
		{
			line++;
			
			for(i=0;!isspace(*line) && *line!='\0';i++)
			{
				label[i] = *line;
				line++;
			}
			
			label[i] = '\0';
			data = atoi(label);
			to_inst(DATA_TYPE, opcode, addressing1, addressing2, data, ABSOLUTE);
			IC++;
		}
		
		/* if second operand is label */
		if(addressing2 == LABEL_ADDRESS)
		{
			for(i=0;!isspace(*line) && *line!='\0';i++)
			{
				label[i] = *line;
				line++;
			}
			
			label[i] = '\0';
			symbol = search_sym(label);
			
			if(symbol == NULL)
			{
				ERROR = NDEF_LABEL_ERROR;
				return;
			}
			
			/* if the label is external */
			else if(symbol->ext_flag == YES)
			{
				to_ent_ext(label, IC, symbol->ext_flag);
				to_inst(LABEL_TYPE, opcode, addressing1, addressing2, NO, EXTERNAL);
				IC++;
			}
			
			else
			{
				to_inst(LABEL_TYPE, opcode, addressing1, addressing2, symbol->address, RELOCATABLE);
				IC++;
			}
		}
		
		/* if second operand is matrix */
		if(addressing2 == MAT_ADDRESS)
		{
			for(i=0;*line!='[';i++)
			{
				label[i] = *line;
				line++;
			}
			
			label[i] = '\0';
			symbol = search_sym(label);
			
			if(symbol == NULL)
			{
				ERROR = NDEF_LABEL_ERROR;
				return;
			}
			
			/* if the label is external */
			else if(symbol->ext_flag == YES)
			{
				to_ent_ext(label, IC, symbol->ext_flag);
				to_inst(LABEL_TYPE, opcode, addressing1, addressing2, NO, EXTERNAL);
				IC++;
			}
			
			else
			{
				to_inst(LABEL_TYPE, opcode, addressing1, addressing2, symbol->address, RELOCATABLE);
				IC++;
			}
			
			line += 2;
			reg_src = *line - '0';
			line += 4;
			reg_dst = *line - '0';
			to_inst(REG_TYPE, opcode, reg_src, reg_dst, data, ABSOLUTE);
			IC++;
		}
		
		/* if second operand is a register */
		if(addressing2 == REG_ADDRESS)
		{
			reg_dst = *(line+1) - '0';
			to_inst(REG_TYPE, opcode, reg_src, reg_dst, data, ABSOLUTE);
			IC++;
		}
	}
}