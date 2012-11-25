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
 * not match type t2 (param 3). Param 1 can also be an operator, or param 2
 * a string list of types.
 * The cond variant is used when a predicate is not given in a conditional
 * expression.
 */
void error_type_mismatch(const string&, int, int);
void error_type_mismatch(int, int, int);
void error_type_mismatch(int, int, const string&);
void error_type_mismatch_cond(int);

/*
 * Prints an error stating that no program entry method ('hatta') was detected.
 */
void error_no_entry();

/*
 * Prints an error saying that an identifier is not a function or procedure.
 */
void error_not_func(const string&);

/*
 * Prints an error that function name (param1) expected n arguments (param2)
 * but got m args (param 3). If a type value is given for param1, resolve it to
 * a string first.
 */
void error_num_args(const string&, int, int);
void error_num_args(int, int, int);

/*
 * Prints an error that input was attempted on a variable (param 1) of type 
   param 2 that cannot accept input.
 */
void error_input_type(const string&, int);

/*
 * Prints an error that a var string (param 1) of type param 2 is not an array.
 */
void error_not_array(const string&, int);

#endif
