/*
 ============================= Maman 14 =====================================
 File Name   : first_pass.h
 Semester    : 2017b
 Description : This file contains the first_pass function. this funcion runs the first scan on the .as file,
 the purpose of the first pass is to check the validation of each line, calculate the file's memmory consumption and
 insert each symbol to a symbol table.
 ============================================================================
*/

#ifndef FIRST_PASS_H_
#define FIRST_PASS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tables.h"
#include "read_line.h"
#include "consts.h"
#include "errors.h"
#include "global_vars.h"
#include "second_pass.h"

/* the first pass function */
void first_pass(char *);

#endif