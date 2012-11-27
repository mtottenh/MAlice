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
too 		  { tokenpos += yyleng; yylval.values.string = yytext;  return TOO;}
became	 	  { tokenpos += yyleng; yylval.values.string = yytext;  return BECAME; }
number		  { tokenpos += yyleng; yylval.values.string = yytext;  return TNUMBER; }
letter		  { tokenpos += yyleng; yylval.values.string = yytext;  return TCHAR; }
sentence	{ tokenpos += yyleng; yylval.values.string = yytext;  return TSTRING; }
ate		  { tokenpos += yyleng; yylval.values.string = yytext;  return INC;	  }
drank 		  { tokenpos += yyleng; yylval.values.string = yytext;  return DEC;	  }
contained     { tokenpos += yyleng; yylval.values.string = yytext;  return CONTAINED; }
had		  { tokenpos += yyleng; yylval.values.string = yytext;  return HAD; }
what	  { tokenpos += yyleng; yylval.values.string = yytext;  return WHAT; }
was		  { tokenpos += yyleng; yylval.values.string = yytext;  return WAS; }
a		  { tokenpos += yyleng; yylval.values.string = yytext;  return A; }
\?		  { tokenpos += yyleng; yylval.values.string = yytext;  return QUESTIONMARK; }
\!		  { tokenpos += yyleng; yylval.values.string = yytext;  return LNOT; }
\!\=	  { tokenpos += yyleng; yylval.values.string = yytext;  return LNOTEQU; }
eventually	  { tokenpos += yyleng; yylval.values.string = yytext;  return EVENTUALLY; }
because		  { tokenpos += yyleng; yylval.values.string = yytext;  return BECAUSE; }
enough 	  { tokenpos += yyleng; yylval.values.string = yytext;  return ENOUGH; }
times 	  { tokenpos += yyleng; yylval.values.string = yytext;  return TIMES; }
perhaps		  { tokenpos += yyleng; yylval.values.string = yytext;  return IF; }
so		  { tokenpos += yyleng; yylval.values.string = yytext;  return THEN; }
or		  { tokenpos += yyleng; yylval.values.string = yytext;  return ELSE; }
either		  { tokenpos += yyleng; yylval.values.string = yytext;  return IF; }
maybe		  { tokenpos += yyleng; yylval.values.string = yytext;  return MAYBE; }
unsure 		{tokenpos += yyleng; yylval.values.string = yytext;  return UNSURE; }
which		{tokenpos += yyleng; yylval.values.string = yytext;  return WHICH; }
of		{tokenpos += yyleng; yylval.values.string = yytext;  return OF; }
found		{tokenpos += yyleng; yylval.values.string = yytext;  return FOUND; }
Alice	{tokenpos += yyleng; yylval.values.string = yytext;  return ALICE; }
spider	{tokenpos += yyleng; yylval.values.string = yytext;  return TREF; }
 /* logical operators (e.g. used in loop predicates) */

&&		 { tokenpos += yyleng; yylval.values.string = yytext;  yylval.values.string = strdup(yytext); return LAND; }
\|\|		 { tokenpos += yyleng; yylval.values.string = yytext;  return LOR; }
==		 { tokenpos += yyleng; yylval.values.string = yytext;  return LEQU; }
\<		 { tokenpos += yyleng; yylval.values.string = yytext;  return LLTHAN; }
\<=		 { tokenpos += yyleng; yylval.values.string = yytext;  return LLTHANEQ; }
\>=		 { tokenpos += yyleng; yylval.values.string = yytext;  return LGTHANEQ; }
\>		 { tokenpos += yyleng; yylval.values.string = yytext;  return LGTHAN; }

 /* functions and procedurs*/
The	  { tokenpos += yyleng; yylval.values.string = yytext;   return THE; }
room {tokenpos += yyleng; yylval.values.string = yytext;  return ROOM; }
looking {tokenpos += yyleng; yylval.values.string = yytext;  return LOOKING; }
\-		{tokenpos += yyleng; yylval.values.string = yytext;  return DASH; }
glass 	{tokenpos += yyleng; yylval.values.string = yytext;  return GLASS; }
  /* Array acess patterns */
\'s		{ tokenpos += yyleng; yylval.values.string = yytext;  return ARRINDO; }
piece		{ tokenpos += yyleng; yylval.values.string = yytext;  return ARRINDC; }
  /* Print statements */
said		{tokenpos += yyleng; yylval.values.string = yytext;  return SAID; }
spoke { tokenpos +=yyleng; yylval.values.string = yytext;  return SPOKE; }

  /* Char literals */
{charlit}		{ tokenpos +=yyleng; yylval.values.string = yytext;
					 return CHARLIT; }

  /* String literals */
{stringlit}		{ tokenpos += yyleng; yylval.values.string = yytext;
					return STRINGLIT; }

 /* Scope blocks */
opened		 { tokenpos += yyleng; yylval.values.string = yytext;  return OBRACE; }
closed		 { tokenpos += yyleng; yylval.values.string = yytext;  return CBRACE; }
{separator}	 { tokenpos += yyleng; yylval.values.string = yytext;
		    return SEPARATOR; }

{string}	 { tokenpos += yyleng;yylval.values.string = yytext;
		   return STRING; }

{integer}	{ tokenpos += yyleng; yylval.values.token = atoi(yytext); return INTEGER; }
\.	 	{ tokenpos += yyleng; yylval.values.string = yytext;  return NULLTOK; }
\,		{ tokenpos += yyleng; yylval.values.string = yytext;  return COMMA;	}
\+		{ tokenpos += yyleng; yylval.values.string = yytext;  return PLUS; }
\*		{ tokenpos += yyleng; yylval.values.string = yytext;  return MULT; }
\/		{ tokenpos += yyleng; yylval.values.string = yytext;  return DIV; }
\%		{ tokenpos += yyleng; yylval.values.string = yytext;  return MOD; }
\^		{ tokenpos += yyleng; yylval.values.string = yytext;  return XOR; }
&		{ tokenpos += yyleng; yylval.values.string = yytext;  return AND; }
\|		{ tokenpos += yyleng; yylval.values.string = yytext;  return OR; }
~		{ tokenpos += yyleng; yylval.values.string = yytext;  return NOT; }
\(		{ tokenpos += yyleng; yylval.values.string = yytext;  return OBRACKET; }
\)		{ tokenpos += yyleng; yylval.values.string = yytext;  return CBRACKET; }

{whitespace}	{ tokenpos += yyleng; yylval.values.string = yytext; } /* ignore any whitespace */
(###[^\n]*)		{}
.		{ tokenpos += yyleng; yylval.values.string = yytext;  yyerror(error_message);}


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
