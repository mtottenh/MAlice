%{
#include <stdio.h>
#include <string.h>
%}

%token CHAR
%token STRING
%token INTEGER
%token SEPERATOR
%token TERMINATOR

%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UNARY

%%

DeclarationList
	: Declaration DeclarationList
	| Declaration
	;

Declaration
	: VarDeclaration
	| Function
	| Procedure
	;

Function
	: "The room" Identifier "(" ParamList ") contained a" Type Codeblock
	;

Procedure
	:  "The looking-glass" Identifier "(" ParamList ")" Codeblock
	;

ParamList
	: Paramater
	| Paramater ',' ParamList
	;
Paramater
	: Type Identifier
	;

/* Brackets Relational Operators
*/
LogExp
	: LogExp '&' Exp 
	| LogExp '|' Exp 
	| LogExp '^' Exp
	| Exp
	;
Exp
	: Exp '+' Term
	| Exp '-' Term
	| Term
	;
Term
	: Term '*' Factor
	| Term '/' Factor
	| Factor
	;
Factor
	: '~' Value %prec UNARY
	| '-' Value %prec UNARY
	| Value
	;
Value
	: INTEGER
	| Identifier
	;
/* Add Array/String type */
Type
	: "number"
	| "letter"
	| "spider" Type
	;

VarDeclaration
	: Identifier "was a" Type
	;

/* Array Initialisation/lval/rval, Result of functions*/
Assignment
	: Identifier "became" LogExp
	| Identifier "became" CHAR
	;

/* Clean up the 'print' grammar, to include spoke, also it only works with logExp*/
Print
	: "said Alice"
	;

/* Add rules for Conditoinals/loop/ArrayAcess/FuncandProcedureCalls
 * user input / null statement / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator
	| Assignment Separator
	| LogExp Print Separator
	;

/* Clean up codeblock grammar, there must be a way to collapse statement line/list */

Codeblock
	: "opened" StatementList "closed"
	; 

StatementList
	: Statement StatementList
	| Statement 
	;
/*
StatementLine
	: Statement StatementLine
	| Statement
	;
*/
Separator
	: ','
	| TERMINATOR
	| "and"
	| "but"
	;
Identifier
	: STRING
	;

%%
main()
{
 return(yyparse());
}

yyerror(s)
char *s;
{
  fprintf(stderr, "%s\n",s);
}

yywrap()
{
  return(1);
}

