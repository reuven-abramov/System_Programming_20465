/*
 ============================= Maman 14 =====================================
 File Name   : second_pass.h
 Semester    : 2017b
 Description : the second pass purpose is to encode each instruction and 
			   it's addresses, convert each instruction to strange four base 
			   counting and print it to the correct files.
 ============================================================================
*/

#ifndef SECOND_PASS_H_
#define SECOND_PASS_H_

#include "global_vars.h"
#include "consts.h"
#include "errors.h"
#include "read_line.h"
#include "tables.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the second pass of the project */
void second_pass(FILE *, char *);

/* the function reads each line, checks its validation and incoding it */
void read_line2(char *,FILE *,FILE *,FILE *);

#endif