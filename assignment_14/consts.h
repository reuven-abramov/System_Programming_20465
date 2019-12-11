/*
 ============================= Maman 14 =====================================
 File Name   : consts.h
 Semester    : 2017b
 Description : This file contains the constants for the project
 ============================================================================
*/

#ifndef CONSTS_H_
#define CONSTS_H_

#define FILE_NAME_LEN 65535
#define MAX_LINE_LEN 81
#define MAX_FILE_LEN 356
#define BUFF_SIZE 1000
#define MAX_LABEL_LEN 31
#define MAX_DIRECTIVE_LEN 7
#define MAX_INSTRACTION_LEN 5
#define MAX_INTEGER_LEN 4
#define MAX_OPERAND_LEN 40
#define MASK_2BIT 3
#define NON -1
#define INST_SIZE 15
#define DIR_SIZE 5
#define IC_START 100
#define MAX_DATA_INT 511
#define MIN_DATA_INT -512
#define MAX_IMM_INT 127
#define MIN_IMM_INT -128
#define REG_LEN 2

enum{NO, YES};
enum{ABSOLUTE, EXTERNAL, RELOCATABLE};
enum{DATA_DIR, STRING_DIR, MAT_DIR,ENTRY_DIR, EXTERN_DIR};
enum{IMM_ADDRESS, LABEL_ADDRESS, MAT_ADDRESS, REG_ADDRESS};
enum{INST_TYPE, REG_TYPE, DATA_TYPE, LABEL_TYPE};
enum{MOV_INST, CMP_INST, ADD_INST, SUB_INST, NOT_INST, CLR_INST, 
	LEA_INST,INC_INST, DEC_INST, JNP_INST, BNE_INST, RED_INST, 
	PRN_INST, JSR_INST, RTS_INST, STOP_INST};
enum{NO_ERROR, SYNTAX_ERROR, LABEL_INST_ERROR, LABEL_DIR_ERROR, 
	DEF_LABEL_ERROR, LONG_LABEL_ERROR, DIR_ERROR, INST_ERROR, 
	FCHAR_LABEL_ERROR, LONG_LINE_ERROR, BAD_ARG_ERROR, ONLY_LABEL_ERROR, 
	ADD_ERROR, LABEL_REG_ERROR, NDEF_LABEL_ERROR, NO_ARG_ERROR};

#endif