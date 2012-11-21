#ifndef __SEMANTICERRORS_H__
#define __SEMANTICERRORS_H__

#include <string>
#include <iostream>
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

#endif
