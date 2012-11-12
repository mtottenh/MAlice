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
{whitespace}	{ tokenpos += yyleng;} /* ignore any whitespace */
\n.*		{ strncpy(linebuf, yytext+1,sizeof(linebuf));
			 /*save the next line*/
		  lineno++; yyless(1); tokenpos = 0; } 
			/*rescan all but the \n*/
.		{ tokenpos += yyleng; yyerror("Lexer Error");}

  /* fprintf(stderr, "Line: %d Unrecognised token :%s\n", 
          yylineno, yytext ); } */

%%

void yyerror (char *s) {
	fprintf(stderr, "%d: %s:\n%s\n", lineno, s, linebuf);
	fprintf(stderr, "%*s\n", tokenpos, "^");
}

main()
{
	int type;
	/* need to figure out how to get line1 recognized */
	do {
		type = yylex();
		fprintf(stderr, "%d: Token: %s Type:%d\n",lineno,yytext,type);
	} while(type != 0);
	
}
