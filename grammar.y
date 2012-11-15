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
%token <token> OF WAS A PROCEDURE FUNC BECAME INC DEC CONTAINEDA HAD WHATWAS QUESTIONMARK
EVENTUALLY BECAUSE ENOUGHTIMES THEN ELSE IF ENDIF MAYBE TOO

/* Primitives */
%token CHAR STRING INTEGER STRINGLIT
%token SEPARATOR NULLTOK COMMA QUOTE
/*Operators */
%token <token> MINUS PLUS MULT DIV MOD 
%token <token> XOR AND OR NOT
/* Logical operators */
%token <token> LAND LOR LEQU LNOT LGTHAN LGTHANEQ LLTHAN LLTHANEQ
/* bracket and braces */
%token <token> OBRACKET CBRACKET ARRINDO ARRINDC
%token <token> USCORE
%token <token> OBRACE CBRACE 

/*Built in functions */
%token <token> PRINT

%left UNARY

%union {
	std::string *string;
	int token;
	Node *node;
	NCodeBlock *block;
	NExpression *exp;
	NStatement *stat;
	NIdentifier *id;
	NVariableDeclaration *var_dec;
	std::vector<NVariableDeclaration *> *paramlist;
	std::vector<NStatement *> *statlist;
	
}
%%

DeclarationList
	: Declaration DeclarationList { /*code here */ }
	| Declaration { /*$$ = $1; */ } 
	;

Declaration
	: VarDeclaration Separator { /*$$ = $1*/ }
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
	;
/* Add Array/String type */
Type
	: TNUMBER {}
	| TCHAR {}
	| TSTRING {}
	| TPTR Type {}
	;

VarDeclaration
	: Identifier WAS A Type {printf("Var Declared\n");/*add id to sym table*/}
	| Identifier HAD Identifier Type {/*arrays init*/}
	;

/* Array Initialisation/lval/rval, Result of functions*/
Assignment
	: Identifier BECAME BitExp   {/*update value in symbol table*/}
	| Identifier BECAME CHAR {/*also need to check errors*/}
	| Identifier BECAME ArrayVal
	| ArrayVal BECAME BitExp
	| ArrayVal BECAME CHAR
	;

VarDeclarationAssignment
	: VarDeclaration OF INTEGER
	;

/* change value to be bit exp */
ArrayVal
	: Identifier ARRINDO Value ARRINDC {/*value is an index*/}
	;
 
/*Print Token matches said allice and spoke*/
Print 
	: PRINT  {/**/}
	;

/* Add rules for /ArrayAcess/FuncandProcedureCalls
 * user input / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator {printf("Statement end\n");} 
	| VarDeclarationAssignment Separator {}
	| Read {}
	| Conditional {}
	| Loop {}
	| Call {}
	| ProcedureDec {}
	| FunctionDec {}
	| NULLTOK {}
	| Increment Separator {}
	| Decrement Separator {}
	| Codeblock {}
      /*| Generalise Print */
	| Assignment Separator {}
	| BitExp Print Separator {}
	| STRINGLIT Print Separator {}
	;

Call 
	: Identifier OBRACKET ParamList CBRACKET {}
	;
ParamList
	: Paramater COMMA ParamList
	| Paramater
	;
Paramater
	: Call {}
	| Identifier {}
	| STRINGLIT {}
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
	: IF OBRACKET Predicate CBRACKET THEN Statement Maybe
	;

Maybe
	: ELSE Statement ENDIF
	| ENDIF
	| ELSE MAYBE OBRACKET Predicate CBRACKET Maybe
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
