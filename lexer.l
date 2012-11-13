%{
#include "y.tab.h"
#include <stdio.h>

int lineno = 1;
char linebuf[500];
int tokenpos = 0;
%}

%option nounput yylineno
char		[a-zA-Z]
separator	(,|and|but|then)
string 		{char}[a-zA-Z0-9]*
integer		[0-9]+
terminator	(too)?\.
whitespace	[ \t]+  

%%

  /*need to formulate rules for ( ) * / % ^ & | - _ ~ to be turned into tokens*/

 /* Keywords */
became	 	  { tokenpos += yyleng; return BECAME; }
was	 	  { tokenpos += yyleng; return WAS; }
number		  { tokenpos += yyleng; return TNUMBER; }
letter		  { tokenpos += yyleng; return TCHAR; }
a		  { tokenpos += yyleng; return A;	  }
 /* functions and procedurs*/
"The room"	  { tokenpos += yyleng;  return FUNC; }
"The looking-glass" {tokenpos += yyleng; return PROCEDURE; }
  /* Array acess patterns */
\'s		{ tokenpos += yyleng; return ARRINDO; }
piece		{ tokenpos += yyleng; return ARRINDC; }
  /* Print statements */
("said Alice"|spoke) { tokenpos +=yyleng; return PRINT; }

  /* Char literals */
\'.\'		{ tokenpos +=yyleng; return CHAR; }

 /* Scope blocks */
opened		 { tokenpos += yyleng; return OBRACE; }
closed		 { tokenpos += yyleng; return CBRACE; }
{separator}	 { tokenpos += yyleng; yylval.string=strdup(yytext);
		    return SEPARATOR; }

{string}	 { tokenpos += yyleng; yylval.string = strdup(yytext);
		   return STRING; }

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


%%

void yyerror (char *s) {
	fprintf(stderr, "%d: %s:\n%s\n", lineno, s, linebuf);
	fprintf(stderr, "%*s\n", tokenpos, "^");
}
/*
main()
{
	int type;
	 need to figure out how to get line1 recognized 
	do {
		type = yylex();
		fprintf(stderr, "%d: Token: %s Type:%d\n",lineno,yytext,type);
	} while(type != 0);
	
}*/
