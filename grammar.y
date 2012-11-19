%{
#include <stdio.h>
#include <string.h>
//#include "node.h"
extern void yyerror(char*);
extern int yylex();

%}

/* Type Tokens */
%token TCHAR TSTRING TPTR TNUMBER  
/* Alice Keywords */
%token OF WAS A PROCEDURE FUNC BECAME INC DEC CONTAINEDA HAD WHATWAS 
QUESTIONMARK EVENTUALLY BECAUSE ENOUGHTIMES THEN ELSE IF ENDIF MAYBE TOO
FOUND

/* Primitives */
%token CHAR STRING INTEGER STRINGLIT
%token SEPARATOR NULLTOK COMMA QUOTE
/*Operators */
%token MINUS PLUS MULT DIV MOD 
%token XOR AND OR NOT
/* Logical operators */
%token LAND LOR LEQU LNOT LGTHAN LGTHANEQ LLTHAN LLTHANEQ
/* bracket and braces */
%token OBRACKET CBRACKET ARRINDO ARRINDC
%token OBRACE CBRACE 

/*Built in functions */
%token PRINT

%left UNARY

%union {
	char *string;
	int val;
}
%%

DeclarationList
	: Declaration DeclarationList { /*code here */ }
	| Declaration { /*$$ = $1; */ } 
	;

Declaration
	: VarDeclaration Separator { /*$$ = $1*/ }
	| VarDeclarationAssignment Separator {}
	| FunctionDec	 {  }
	| ProcedureDec     { /*$$ = $1*/ }
	;

FunctionDec
	: FUNC Identifier OBRACKET ParamListDec CBRACKET CONTAINEDA Type Codeblock
	{/*$$ = new funcNode($2,$7,$8,$4) name/type/body/args*/}
	| FUNC Identifier OBRACKET CBRACKET CONTAINEDA Type Codeblock {}
	;

ProcedureDec
	:  PROCEDURE Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ /*$$ = new procNode( $2,$6,$4) name/body/args */ }
	|  PROCEDURE Identifier OBRACKET CBRACKET Codeblock {}
	;

ParamListDec
	: ParamaterDec {/*create a std::vector */}
	| ParamaterDec COMMA ParamListDec { /* $$ = param_vector.pushback($1)*/}
	;
ParamaterDec
	: Type Identifier {/**/}
	;

  /* Brackets Relational Operators*/
BitExp
	: Exp AND BitExp {}
	| Exp OR BitExp  {}
	| Exp XOR BitExp {}
	| OBRACKET BitExp CBRACKET {}
	| Exp {}
	;
Exp
	: Exp PLUS Term {}
	| Exp MINUS Term {}
	| Term {}
	;
Term
	: Term MULT Factor {}
	| Term DIV Factor {}
	| Term MOD Factor {}
	| Factor {}
	;
Factor
	: NOT Value %prec UNARY {}
	| MINUS Value %prec UNARY {}
	| Value {}
	;
Value
	: INTEGER {}
	| Identifier {}
	| Call {}
	| ArrayVal {}
	;
/* Add Array/String type */
Type
	: TNUMBER {}
	| TCHAR {}
	| TSTRING {}
	| TPTR Type {}
	;

VarDeclaration
	: Identifier WAS A Type {printf("Var Declared\n");}
	| Identifier HAD BitExp Type {printf("Array Declared\n"); }
	;

Assignment
	: Identifier BECAME CHAR {/*also need to check errors*/}
	| Identifier BECAME BitExp
	| ArrayVal BECAME BitExp
	| ArrayVal BECAME CHAR
	;

VarDeclarationAssignment
	: VarDeclaration OF BitExp {printf("and assigned\n");}
	| VarDeclaration OF CHAR	{printf("and assigned\n");}
	| VarDeclaration OF STRINGLIT {printf("and assigned\n");}
	;

/* change value to be bit exp */
ArrayVal
	: Identifier ARRINDO Value ARRINDC {/*value is an index*/}
	;
 
/*Print Token matches said allice and spoke*/
Print 
	: PRINT  {/**/}
	;

Return
	: FOUND BitExp
	;

/* Add rules for /ArrayAcess/FuncandProcedureCalls
 * user input / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator {} 
	| VarDeclarationAssignment Separator {}
	| Read {}
	| Conditional {}
	| Loop {}
	| Call Separator {}
	| ProcedureDec {}
	| FunctionDec {}
	| NULLTOK {}
	| Increment Separator {}
	| Decrement Separator {}
	| Codeblock {}
      /*| Generalise Print */
	| Assignment Separator {printf("Variable assigned\n");}
	| BitExp Print Separator {}
	| STRINGLIT Print Separator {}
	| CHAR Print Separator {}
	| Return Separator {}
	;

Call 
	: Identifier OBRACKET ParamList CBRACKET {}
	| Identifier OBRACKET CBRACKET {printf("Called\n");}
	;
ParamList
	: Paramater COMMA ParamList
	| Paramater
	;
Paramater
	: STRINGLIT {}
	| CHAR {}
	| BitExp {}
	;
Read
	: WHATWAS Identifier QUESTIONMARK
	;
Loop
	: EVENTUALLY OBRACKET Predicate CBRACKET BECAUSE StatementList ENOUGHTIMES
	;

Predicate
	: PredPrime LOR Predicate
	| PredPrime LAND Predicate
	| PredPrime
	| LNOT OBRACKET Predicate CBRACKET
	| OBRACKET Predicate CBRACKET
	;

PredPrime
	: BitExp LEQU BitExp
	| BitExp LLTHAN BitExp
	| BitExp LLTHANEQ BitExp
	| BitExp LGTHAN BitExp
	| BitExp LGTHANEQ BitExp
	;

Increment
	: Identifier INC
	;

Decrement
	: Identifier DEC
	;	

Conditional
	: IF OBRACKET Predicate CBRACKET THEN StatementList Maybe
	;

Maybe
	: ELSE StatementList ENDIF
	| ENDIF
	| ELSE MAYBE OBRACKET Predicate CBRACKET THEN StatementList Maybe
	;

Codeblock
	: OBRACE StatementList CBRACE {/* create new scoping symtable + vector*/}
	; 

StatementList
	: StatementList Statement {}
	| Statement{}
	;

Separator
	: NULLTOK {}
	| SEPARATOR {}
	| COMMA {}
	;

Identifier
	: STRING {printf("ID: %s\n", yyval.string);}
	;

%%
int main()
{
 int node = yyparse();
 return node;
}
/*
yyerror(s)
char *s;
{
  fprintf(stderr, "%s\n",s);
}

yywrap()
{
  return(1);
}
*/
