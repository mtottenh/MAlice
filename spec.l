%{
//#include "y.tab.h"
#include <stdio.h>

#define CHAR 1
#define STRING 2
#define INTEGER 3
#define TERMINATOR 4
#define SEPERATOR 5
int lineno = 1;
char linebuf[500];
int tokenpos = 0;
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
{char}		{ tokenpos += yyleng; return CHAR;}
{seperator}	{ tokenpos += yyleng; return SEPERATOR; }
{string}	{ tokenpos += yyleng; return STRING; }
{integer}	{ tokenpos += yyleng; return INTEGER; }
{terminator} 	{ tokenpos += yyleng; return TERMINATOR; }
{whitespace}	/* ignore any whitespace */
\n		{ lineno++; tokenpos = 0; }
.		{ fprintf(stderr, "Line: %d Unrecognised token :%s\n", 
								yylineno, yytext ); }

%%

void yyerror (char *s) {
	printf("Line %d: %s:\n%s\n", lineno, s, linebuf);
	printf("%*s\n", 1+tokenpos, "^");
}

main()
{
	int type;
	do {
		type = yylex();
		fprintf(stderr, "Line: %d Varname: %s Type: %d\n",lineno,yytext,type);
	} while(type != 0);
	
}
