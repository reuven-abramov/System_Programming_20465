/*
 ============================= Maman 14 =====================================
 File Name   : errors.h
 Semester    : 2017b
 Description : This file contains the check_errors function that checks if the ERROR varible contains error number
				and prints the error on the screen.
 ============================================================================
*/

#ifndef ERRORS_H_
#define ERRORS_H_

#include <stdio.h>
#include "global_vars.h"
#include "consts.h"

/* returns true if the was an error and prints the error on the screen */
int check_errors();

#endif