#ifndef __SEMANTICERRORS_H__
#define __SEMANTICERRORS_H__

#include <string>
#include <iostream>
#include "TypeMap.hpp"

using namespace std;

/*
 * Prints an error that the variable exists in current scope. Takes the name
 * of the offending variable. 
 */
void error_var_exists(const string&);

/*
 * Prints an error that the variable given is a keyword. Takes the name of the
 * offending variable.
 */
void error_keyword(const string&);

/*
 * Prints an error that a variable does not exist. Takes the name of the
 * offending variable.
 */
void error_var_not_found(const string&);

/*
 * Prints an error stating that a variable (param 1) of type t1 (param 2) does
 * not match type t2 (param 3).
 */
void error_type_mismatch(const string&, int, int);

#endif
