/*
 ============================= Maman 14 =====================================
 File Name   : global_vars.h
 Semester    : 2017b
 Description : This file contains all the global variables and the structs of the project.
 ============================================================================
*/

#ifndef GLOBAL_VARS_H_
#define GLOBAL_VARS_H_

int IC;
int DC;
int EC;
int LN;
int ERROR;

typedef struct keywords 
{
	char *word;
} keywords;

typedef struct data_node
{
	int address;
	int data : 10;
	struct data_node *next;
} data_node;

typedef struct symbol_node
{
	int address;
	char *symbol;
	int ext_flag;
	int inst_flag;
	struct symbol_node *next;
} symbol_node;

typedef struct instruction_node
{
	int insttype;
	int address : 8;
	unsigned int era : 2;
	struct instruction_node *next;
	
	union type
	{
		struct inst
		{
			unsigned int opcode : 4;
			unsigned int src : 2;
			unsigned int dst : 2;
		} inst;
		
		struct reg
		{
			unsigned int src : 4;
			unsigned int dst : 4;
		} reg;
		
		struct data
		{
			int value : 8;
		} data;
		
		struct label
		{
			unsigned int address : 8;
		} label;
	} type;
} instruction_node;

typedef struct ent_ext_node
{
	char *label;
	int address;
	int ext_flag;
	struct ent_ext_node *next;
} ent_ext_node;

#endif