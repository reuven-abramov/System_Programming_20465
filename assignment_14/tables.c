/*
 ============================= Maman 14 =====================================
 File Name   : tables.c
 Semester    : 2017b
 Description : This file contains functions for handling the data stractures 
			   of the project.
 ============================================================================
*/

#include "tables.h"

data_node *data_head = NULL;
symbol_node *symbol_head = NULL;
instruction_node *instruction_head = NULL;
ent_ext_node *ent_ext_head = NULL;

/* creates new node for directive data table and initializing it */
data_node *new_data(int data)
{
	data_node *new = (data_node *)malloc(sizeof(data_node));
	new->address = DC;
	new->data = data;
	new->next = NULL;
	return new;
}

/* creates new node for symbol table and initializing it */
symbol_node *new_symbol(char *symbol, int address, int ext_flag, int inst_flag)
{
	symbol_node *new = (symbol_node *)malloc(sizeof(symbol_node));
	new->address = address;
	new->symbol = malloc(strlen(symbol));
	strcpy(new->symbol,symbol);
	new->ext_flag = ext_flag;
	new->inst_flag = inst_flag;
	new->next = NULL;
	return new;
}

/* creates new node for instructions table and initializing it */
instruction_node *new_inst(int insttype, int opcode, int src, int dst, int data, int era)
{
	instruction_node *new = (instruction_node *)malloc(sizeof(instruction_node));
	new->insttype = insttype;
	new->address = IC;
	new->era = era;
	new->next = NULL;
	
	if(insttype == INST_TYPE)
	{
		new->type.inst.opcode = opcode;
		new->type.inst.src = src;
		new->type.inst.dst = dst;
	}
	
	else if(insttype == REG_TYPE)
	{
		new->type.reg.dst = dst;
		new->type.reg.src = src;
	}
	
	else if(insttype == DATA_TYPE)
	{
		new->type.data.value = data;
	}
	
	else if(insttype == LABEL_TYPE)
	{
		new->type.label.address = data;
	}
	
	return new;
}

/* creates new node for entry or extern labels table and initializing it */
ent_ext_node *new_ent_ext(char *label, int address, int ext_flag)
{
	ent_ext_node *new = (ent_ext_node *)malloc(sizeof(ent_ext_node));
	new->address = address;
	new->label = malloc(strlen(label));
	strcpy(new->label,label);
	new->ext_flag = ext_flag;
	new->next = NULL;
	return new;
}

/* adds data to directive data table */
void to_data(int data)
{
	data_node *temp = data_head;
	data_node *new = new_data(data);
	DC++;
	
	if(data_head == NULL)
	{
		data_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* adds symbol to symbol table */
void to_symbol(char *symbol, int address, int ext_flag, int inst_flag)
{
	symbol_node *temp = symbol_head;
	symbol_node *new = new_symbol(symbol, address, ext_flag, inst_flag);
	
	if(symbol_head == NULL)
	{
		symbol_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* adds instruction to instruction table */
void to_inst(int insttype, int opcode, int src, int dst, int data, int era)
{
	instruction_node *temp = instruction_head;
	instruction_node *new = new_inst(insttype, opcode, src, dst, data, era);
	
	if(instruction_head == NULL)
	{
		instruction_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* adds label to entry or extern labels table */
void to_ent_ext(char *label, int address, int ext_flag)
{
	ent_ext_node *temp = ent_ext_head;
	ent_ext_node *new = new_ent_ext(label, address, ext_flag);
	
	if(ent_ext_head == NULL)
	{
		ent_ext_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* prints all the tables to the correct files */
void print_to_files(FILE *ob_file, FILE *ent_file, FILE *ext_file)
{
	instruction_node *temp_inst = instruction_head;
	data_node *temp_data = data_head;
	ent_ext_node *temp_ent_ext = ent_ext_head;
	char base4_chars[4] = {'a', 'b', 'c', 'd'};
	char address[5], data[6], ic[5], dc[5];
	char era;
	int mask = MASK_2BIT, i, j;
	
	for(i=0,j=6;i<4;i++,j-=2)
		ic[i] = base4_chars[((IC-99) >> j) & mask];
	ic[4] = '\0';
	
	for(i=0,j=6;i<4;i++,j-=2)
		dc[i] = base4_chars[(DC >> j) & mask];
	dc[4] = '\0';
	
	fprintf(ob_file, "Base 4\t\tBase 4\naddress\t\tcode\n\n\t%s\t%s\n\n", ic, dc);

	
	while(temp_inst != NULL)
	{
		for(i=0,j=6;i<4;i++,j-=2)
			address[i] = base4_chars[(temp_inst->address >> j) & mask];
		address[4] = '\0';

		era = base4_chars[temp_inst->era];
		
		if(temp_inst->insttype == INST_TYPE)
		{
			char opcode[3];
			char src;
			char dst;
			
			for(i=0,j=2;i<2;i++,j-=2)
				opcode[i] = base4_chars[(temp_inst->type.inst.opcode >> j) & mask];
			opcode[2] = '\0';
			
			src = base4_chars[temp_inst->type.inst.src];
			dst = base4_chars[temp_inst->type.inst.dst];
			
			fprintf(ob_file, "\t%s\t%s%c%c%c\n", address, opcode, src, dst, era);
		}
		
		else if(temp_inst->insttype == REG_TYPE)
		{
			char src[3], dst[3];
			
			for(i=0,j=2;i<2;i++,j-=2)
				src[i] = base4_chars[(temp_inst->type.reg.src >> j) & mask];
			src[2] = '\0';
			
			for(i=0,j=2;i<2;i++,j-=2)
				dst[i] = base4_chars[(temp_inst->type.reg.dst >> j) & mask];
			dst[2] = '\0';
		
			fprintf(ob_file, "\t%s\t%s%s%c\n", address, src, dst, era);
		}
		
		else if(temp_inst->insttype == DATA_TYPE)
		{
			char value[5];
			
			for(i=0,j=6;i<4;i++,j-=2)
				value[i] = base4_chars[(temp_inst->type.data.value >> j) & mask];
			value[4] = '\0';
			
			fprintf(ob_file, "\t%s\t%s%c\n", address, value, era);
		}
		
		else if(temp_inst->insttype == LABEL_TYPE)
		{
			char label[5];
			
			for(i=0,j=6;i<4;i++,j-=2)
				label[i] = base4_chars[(temp_inst->type.label.address >> j) & mask];
			label[4] = '\0';
			
			fprintf(ob_file, "\t%s\t%s%c\n", address, label, era);
		}
			
		temp_inst = temp_inst->next;
	}

	while(temp_data != NULL)
	{
		for(i=0,j=6;i<4;i++,j-=2)
			address[i] = base4_chars[(temp_data->address >> j) & mask];
		address[4] = '\0';
		
		for(i=0,j=8;i<5;i++,j-=2)
			data[i] = base4_chars[(temp_data->data >> j) & mask];
		data[5] = '\0';
		
		fprintf(ob_file, "\t%s\t%s\n", address, data);
		temp_data = temp_data->next;
	}
	
	while(temp_ent_ext != NULL)
	{
		for(i=0,j=6;i<4;i++,j-=2)
			address[i] = base4_chars[(temp_ent_ext->address >> j) & mask];
		address[4] = '\0';
		
		if(temp_ent_ext->ext_flag == 1)
			fprintf(ext_file, "%s\t%s\n", temp_ent_ext->label, address);
		else if(temp_ent_ext->ext_flag == 0)
			fprintf(ent_file, "%s\t%s\n", temp_ent_ext->label, address);
		temp_ent_ext = temp_ent_ext->next;
	}
}

/* updates the addressess of the directive data table */
void update_DC()
{
	data_node *data_temp = data_head;
	symbol_node *sym_temp = symbol_head;
	
	while(data_temp != NULL)
	{
		data_temp->address += IC;
		if(data_temp->address > MAX_FILE_LEN)
		{
			ERROR = SYNTAX_ERROR;
			return;
		}
		data_temp = data_temp->next;
	}
	
	while(sym_temp != NULL)
	{
		if(!sym_temp->ext_flag && !sym_temp->inst_flag)
			sym_temp->address += (IC);
		
		sym_temp = sym_temp->next;
	}
}

/* returns node from symbol table with the given label */
symbol_node *search_sym(char *symbol)
{
	symbol_node *temp = symbol_head;
	
	while(temp != NULL)
	{
		if(strcmp(temp->symbol, symbol) == NO)
			return temp;
		
		temp = temp->next;
	}
	
	return NULL;
}

/* free all tables */
void free_tables()
{
	data_node *dat = data_head;
	symbol_node *sym = symbol_head;
	instruction_node *inst = instruction_head;
	ent_ext_node *ent_ext = ent_ext_head;
	
	while(data_head != NULL)
	{
		data_head = data_head->next;
		free(dat);
		dat = data_head;
	}
	
	while(symbol_head != NULL)
	{
		symbol_head = symbol_head->next;
		free(sym);
		sym = symbol_head;
	}
	
	while(instruction_head != NULL)
	{
		instruction_head = instruction_head->next;
		free(inst);
		inst = instruction_head;
	}
	
	while(ent_ext_head != NULL)
	{
		ent_ext_head = ent_ext_head->next;
		free(ent_ext);
		ent_ext = ent_ext_head;
	}
}