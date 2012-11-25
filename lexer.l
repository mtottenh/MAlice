%{

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#define YY_NO_INPUT
#include "Node/NodeIncludes.hpp"

//extern char *strdup(const char *s);
//extern int fileno(FILE *stream);
extern void yyerror(char *s, ...);
int lineno = 1;
char linebuf[500];
int tokenpos = 0;
char error_message[] = {"Lexer Error"};

/* handle locations */
int yycolumn = 1;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;
%}

%option nounput yylineno
char		[a-zA-Z]
separator	(and|but|then)
integer		[0-9]+
charlit		['][^'][']
stringlit   ["][^"]*["]
string 		{char}[a-zA-Z0-9_]*
whitespace	[ \t]+  
%%
 /* Start state */
 
  /*need to formulate rules for ( ) * / % ^ & | - _ ~ to be turned into tokens*/
\n.*		{ strncpy(linebuf, yytext+1,sizeof(linebuf)); 
		  lineno++; yyless(1); tokenpos = 0; yycolumn = 1; } 

 /* Keywords */
too 		  { tokenpos += yyleng; return TOO;}
became	 	  { tokenpos += yyleng; return BECAME; }
number		  { tokenpos += yyleng; return TNUMBER; }
letter		  { tokenpos += yyleng; return TCHAR; }
sentence	{ tokenpos += yyleng; return TSTRING; }
ate		  { tokenpos += yyleng; return INC;	  }
drank 		  { tokenpos += yyleng; return DEC;	  }
contained     { tokenpos += yyleng; return CONTAINED; }
had		  { tokenpos += yyleng; return HAD; }
what	  { tokenpos += yyleng; return WHAT; }
was		  { tokenpos += yyleng; return WAS; }
a		  { tokenpos += yyleng; return A; }
\?		  { tokenpos += yyleng; return QUESTIONMARK; }
\!		  { tokenpos += yyleng; return LNOT; }
\!\=	  { tokenpos += yyleng; return LNOTEQU; }
eventually	  { tokenpos += yyleng; return EVENTUALLY; }
because		  { tokenpos += yyleng; return BECAUSE; }
enough 	  { tokenpos += yyleng; return ENOUGH; }
times 	  { tokenpos += yyleng; return TIMES; }
perhaps		  { tokenpos += yyleng; return IF; }
so		  { tokenpos += yyleng; return THEN; }
or		  { tokenpos += yyleng; return ELSE; }
either		  { tokenpos += yyleng; return IF; }
maybe		  { tokenpos += yyleng; return MAYBE; }
unsure 		{tokenpos += yyleng; return UNSURE; }
which		{tokenpos += yyleng; return WHICH; }
of		{tokenpos += yyleng; return OF; }
found		{tokenpos += yyleng; return FOUND; }
Alice	{tokenpos += yyleng; return ALICE; }
spider	{tokenpos += yyleng; return TREF; }
 /* logical operators (e.g. used in loop predicates) */

&&		 { tokenpos += yyleng; return LAND; }
\|\|		 { tokenpos += yyleng; return LOR; }
==		 { tokenpos += yyleng; return LEQU; }
\<		 { tokenpos += yyleng; return LLTHAN; }
\<=		 { tokenpos += yyleng; return LLTHANEQ; }
\>=		 { tokenpos += yyleng; return LGTHANEQ; }
\>		 { tokenpos += yyleng; return LGTHAN; }

 /* functions and procedurs*/
The	  { tokenpos += yyleng;  return THE; }
room {tokenpos += yyleng; return ROOM; }
looking {tokenpos += yyleng; return LOOKING; }
\-		{tokenpos += yyleng; return DASH; }
glass 	{tokenpos += yyleng; return GLASS; }
  /* Array acess patterns */
\'s		{ tokenpos += yyleng; return ARRINDO; }
piece		{ tokenpos += yyleng; return ARRINDC; }
  /* Print statements */
said		{tokenpos += yyleng; return SAID; }
spoke { tokenpos +=yyleng; return SPOKE; }

  /* Char literals */
{charlit}		{ tokenpos +=yyleng; yylval.string = strdup(yytext);
					 return CHARLIT; }

  /* String literals */
{stringlit}		{ tokenpos += yyleng; yylval.string = strdup(yytext);
					return STRINGLIT; }

 /* Scope blocks */
opened		 { tokenpos += yyleng; return OBRACE; }
closed		 { tokenpos += yyleng; return CBRACE; }
{separator}	 { tokenpos += yyleng; yylval.string = strdup(yytext);
		    return SEPARATOR; }

{string}	 { tokenpos += yyleng; yylval.string = strdup(yytext);
		   return STRING; }

{integer}	{ tokenpos += yyleng; yylval.token = atoi(yytext); return INTEGER; }
\.	 	{ tokenpos += yyleng; return NULLTOK; }
\,		{ tokenpos += yyleng; return COMMA;	}
\+		{ tokenpos += yyleng; return PLUS; }
\*		{ tokenpos += yyleng; return MULT; }
\/		{ tokenpos += yyleng; return DIV; }
\%		{ tokenpos += yyleng; return MOD; }
\^		{ tokenpos += yyleng; return XOR; }
&		{ tokenpos += yyleng; return AND; }
\|		{ tokenpos += yyleng; return OR; }
~		{ tokenpos += yyleng; return NOT; }
\(		{ tokenpos += yyleng; return OBRACKET; }
\)		{ tokenpos += yyleng; return CBRACKET; }

{whitespace}	{ tokenpos += yyleng;} /* ignore any whitespace */
(###[^\n]*)		{}
.		{ tokenpos += yyleng; yyerror(error_message);}


%%

 /*void yyerror (char *s) {
	fprintf(stderr, "line %d: pos %d %s:\n%s\n", lineno, tokenpos, s, linebuf);
	fprintf(stderr, "%*s\n", tokenpos, "^");
}*/
 /*
main()
{
	int type;
	do {
		type = yylex();
		fprintf(stderr, "%d: Token: %s Type:%d\n",lineno,yytext,type);
	} while(type != 0);
 }*/
