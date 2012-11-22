%{

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#define YY_NO_INPUT
#include "Node/NodeIncludes.hpp"

//extern char *strdup(const char *s);
//extern int fileno(FILE *stream);
void yyerror(char *s);
int lineno = 1;
char linebuf[500];
int tokenpos = 0;
char error_message[] = {"Lexer Error"};
%}

%option nounput yylineno
char		[a-zA-Z]
separator	(and|but|then)
integer		[0-9]+
charlit		['][^'][']
stringlit   ["][^"]*["]
string 		{char}[a-zA-Z0-9_]*
whitespace	[ \t]+  
%s INIT
%%
 /* Start state */
 
  /*need to formulate rules for ( ) * / % ^ & | - _ ~ to be turned into tokens*/

 /* Keywords */
too 		  { tokenpos += yyleng; return TOO;}
became	 	  { tokenpos += yyleng; return BECAME; }
"was a"	 	  { tokenpos += yyleng; return WAS; }
number		  { tokenpos += yyleng; return TNUMBER; }
letter		  { tokenpos += yyleng; return TCHAR; }
sentence	{ tokenpos += yyleng; return TSTRING; }
ate		  { tokenpos += yyleng; return INC;	  }
drank 		  { tokenpos += yyleng; return DEC;	  }
"contained a"     { tokenpos += yyleng; return CONTAINEDA; }
had		  { tokenpos += yyleng; return HAD; }
"what was"	  { tokenpos += yyleng; return WHATWAS; }
\?		  { tokenpos += yyleng; return QUESTIONMARK; }
\!		  { tokenpos += yyleng; return LNOT; }
\!\=	  { tokenpos += yyleng; return LNOTEQU; }
eventually	  { tokenpos += yyleng; return EVENTUALLY; }
because		  { tokenpos += yyleng; return BECAUSE; }
"enough times" 	  { tokenpos += yyleng; return ENOUGHTIMES; }
perhaps		  { tokenpos += yyleng; return IF; }
so		  { tokenpos += yyleng; return THEN; }
or		  { tokenpos += yyleng; return ELSE; }
either		  { tokenpos += yyleng; return IF; }
maybe		  { tokenpos += yyleng; return MAYBE; }
"because Alice was unsure which" { tokenpos += yyleng; return ENDIF; }
of		{tokenpos += yyleng; return OF; }
"Alice found"		{tokenpos += yyleng; return FOUND; }
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
"The room"	  { tokenpos += yyleng;  return FUNC; }
"The looking-glass" {tokenpos += yyleng; return PROCEDURE; }
  /* Array acess patterns */
\'s		{ tokenpos += yyleng; return ARRINDO; }
piece		{ tokenpos += yyleng; return ARRINDC; }
  /* Print statements */
("said Alice"|spoke) { tokenpos +=yyleng; return PRINT; }

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
-		{ tokenpos += yyleng; return MINUS; }
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
[\n].*		{ strncpy(linebuf, yytext+1,sizeof(linebuf)); 
		  lineno++; yyless(1); tokenpos = 0; } 
			/*rescan all but the \n*/
.		{ tokenpos += yyleng; yyerror(error_message);}


%%

void yyerror (char *s) {
	fprintf(stderr, "line %d: pos %d %s:\n%s\n", lineno, tokenpos, s, linebuf);
	fprintf(stderr, "%*s\n", tokenpos, "^");
}
 /*
main()
{
	int type;
	do {
		type = yylex();
		fprintf(stderr, "%d: Token: %s Type:%d\n",lineno,yytext,type);
	} while(type != 0);
 }*/
