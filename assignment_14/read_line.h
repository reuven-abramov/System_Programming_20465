/*
 ============================= Maman 14 =====================================
 File Name   : read_line.h
 Semester    : 2017b
 Description : This file contains functions for scanning the lines of the file, reading and decoding each word in the line.
 ============================================================================
*/

#ifndef READ_LINE_H_
#define READ_LINE_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "consts.h"
#include "errors.h"
#include "global_vars.h"
#include "tests.h"
#include "tables.h"

/* reads each line and check its validation */
void read_line(char *);

/* returns true if the line is empty or a comment */
int ignore_line(char *);

/* skipping the space chars before the word */
char *skip_space(char *);

/* jumps to the next word */
char *next_word(char *);

/* jumps to the word after the comma */
char *to_comma(char *);

#endif