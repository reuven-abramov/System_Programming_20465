/*
 ============================= Maman 14 =====================================
 File Name   : tests.h
 Semester    : 2017b
 Description : This file contains functions that tests the validation of the 
			   file.
 ============================================================================
*/

#ifndef TESTS_H_
#define TESTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tables.h"
#include "read_line.h"
#include "errors.h"
#include "consts.h"
#include "global_vars.h"

/* checks if the word is a valid label and put it in symbol table */
int is_label(char *);

/* checks if the word is a directive and returns it's number */
int is_dir(char *);

/* checks if the word is a instruction and returns it's number */
int is_inst(char *);

/* checks the validation of the directive sentence */
int check_dir(char *, int);

/* checks the validation of the instruction sentence */
int check_inst(char *);

/* checks operand addressing */
int check_addressing(char *);

#endif