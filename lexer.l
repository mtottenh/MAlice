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
too 		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return TOO;}
became	 	  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return BECAME; }
number		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return TNUMBER; }
letter		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return TCHAR; }
sentence	{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return TSTRING; }
ate		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return INC;	  }
drank 		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return DEC;	  }
contained     { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return CONTAINED; }
had		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return HAD; }
what	  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return WHAT; }
was		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return WAS; }
a		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return A; }
\?		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return QUESTIONMARK; }
\!		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LNOT; }
\!\=	  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LNOTEQU; }
eventually	  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return EVENTUALLY; }
because		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return BECAUSE; }
enough 	  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return ENOUGH; }
times 	  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return TIMES; }
perhaps		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return IF; }
so		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return THEN; }
or		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return ELSE; }
either		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return IF; }
maybe		  { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return MAYBE; }
unsure 		{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return UNSURE; }
which		{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return WHICH; }
of		{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return OF; }
found		{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return FOUND; }
Alice	{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return ALICE; }
spider	{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return TREF; }
 /* logical operators (e.g. used in loop predicates) */

&&		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  yylval.values.string = strdup(yytext); return LAND; }
\|\|		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LOR; }
==		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LEQU; }
\<		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LLTHAN; }
\<=		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LLTHANEQ; }
\>=		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LGTHANEQ; }
\>		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LGTHAN; }

 /* functions and procedurs*/
The	  { tokenpos += yyleng; yylval.values.string = strdup(yytext);   return THE; }
room {tokenpos += yyleng; yylval.values.string = strdup(yytext);  return ROOM; }
looking {tokenpos += yyleng; yylval.values.string = strdup(yytext);  return LOOKING; }
\-		{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return DASH; }
glass 	{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return GLASS; }
  /* Array acess patterns */
\'s		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return ARRINDO; }
piece		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return ARRINDC; }
  /* Print statements */
said		{tokenpos += yyleng; yylval.values.string = strdup(yytext);  return SAID; }
spoke { tokenpos +=yyleng; yylval.values.string = strdup(yytext);  return SPOKE; }

  /* Char literals */
{charlit}		{ tokenpos +=yyleng; yylval.values.string = strdup(yytext);
					 return CHARLIT; }

  /* String literals */
{stringlit}		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);
					return STRINGLIT; }

 /* Scope blocks */
opened		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return OBRACE; }
closed		 { tokenpos += yyleng; yylval.values.string = strdup(yytext);  return CBRACE; }
{separator}	 { tokenpos += yyleng; yylval.values.string = strdup(yytext);
		    return SEPARATOR; }

{string}	 { tokenpos += yyleng;yylval.values.string = strdup(yytext);
		   return STRING; }

{integer}	{ tokenpos += yyleng; yylval.values.token = atoi(yytext); return INTEGER; }
\.	 	{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return NULLTOK; }
\,		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return COMMA;	}
\+		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return PLUS; }
\*		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return MULT; }
\/		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return DIV; }
\%		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return MOD; }
\^		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return XOR; }
&		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return AND; }
\|		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return OR; }
~		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return NOT; }
\(		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return OBRACKET; }
\)		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  return CBRACKET; }

{whitespace}	{ tokenpos += yyleng; yylval.values.string = strdup(yytext); } /* ignore any whitespace */
(###[^\n]*)		{}
.		{ tokenpos += yyleng; yylval.values.string = strdup(yytext);  yyerror(error_message);}


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
