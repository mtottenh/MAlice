%{
//#include "y.tab.h"
#include <stdio.h>

#define CHAR 1
#define STRING 2
#define INTEGER 3
#define TERMINATOR 4
#define SEPERATOR 5

%}
%option nounput yylineno
char		[a-zA-Z]
seperator	(","|"and"|"but"|"then")
string 		{char}[a-zA-Z0-9]*
integer		[0-9]+
terminator	(too)?\.
whitespace	[ \t]+  
%%

  /*need to formulate rules for ( ) * / % ^ & | - _ ~ to be turned into tokens*/
{char}		{ return CHAR; }
{seperator}	{ return SEPERATOR; }
{string}	{ return STRING; }
{integer}	{ return INTEGER; }
{terminator} 	{ return TERMINATOR; }
{whitespace}	/* ignore any whitespace */
.		{ fprintf(stderr, "Line: %d Unrecognised token :%s\n", 
								yylineno, yytext ); }

%%

main()
{
	int type;
	do {
		type = yylex();
		fprintf(stderr, "Line: %d Varname: %s Type: %d\n",yylineno,yytext,type);
	} while(type != 0);
	
}
