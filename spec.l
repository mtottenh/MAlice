%{
#include "y.tab.h"
#include <stdio.h>

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
opened		{ tokenpos += yyleng; return OBRACE; }
closed		{ tokenpos += yyleng; return CBRACE; }
{string}	{ tokenpos += yyleng; return STRING; }
{integer}	{ tokenpos += yyleng; return INTEGER; }
{terminator} 	{ tokenpos += yyleng; return TERMINATOR; }
\+		{ tokenpos += yyleng; return PLUS; }
-		{ tokenpos += yyleng; return MINUS; }
\*		{ tokenpos += yyleng; return MULT; }
\/		{ tokenpos += yyleng; return DIV; }
%		{ tokenpos += yyleng; return MOD; }
\^		{ tokenpos += yyleng; return XOR; }
&		{ tokenpos += yyleng; return AND; }
\|		{ tokenpos += yyleng; return OR; }
_		{ tokenpos += yyleng; return USCORE; }
~		{ tokenpos += yyleng; return NOT; }
\(		{ tokenpos += yyleng; return OBRACKET; }
\)		{ tokenpos += yyleng; return CBRACKET; }

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
