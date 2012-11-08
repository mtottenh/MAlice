%{
//#include "y.tab.h"
#include <stdio.h>

#define CHAR 0
#define STRING 1
#define INTEGER 2
#define TERMINATOR 3
#define SEPERATOR 4

%}

char		[a-zA-Z]
seperator	("and"|"but"|"then")
string 		{char}[a-zA-Z0-9]*
integer		[0-9]+
terminator	(too)?\.

%%

{char}			{ return CHAR; }
{seperator}		{ return SEPERATOR; }
{string}		{ return STRING; }
{integer}		{ return INTEGER; }
{terminator} 	{ return TERMINATOR; }
.|\n			{ fprintf(stderr, "Unrecognised token :%s at: %d", 
									yytext, yylineno ); }

%%

main()
{
	fprintf(stderr, "%d", yylex());
}
