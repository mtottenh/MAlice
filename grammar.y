%{
#include <stdio.h>
#include <string.h>
extern void yyerror(char*);
extern int yylex();

%}

/* Type Tokens */
%token TCHAR TSTRING TPTR TNUMBER  
/* Alice Keywords */
%token WAS A PROCEDURE FUNC BECAME INC DEC CONTAINEDA HAD WHATWAS QUESTIONMARK
	EVENTUALLY BECAUSE ENOUGHTIMES PERHAPS SO ORKEYWORD EITHER UNSUREALICE

/* Primitives */
%token CHAR STRING INTEGER
%token SEPARATOR TERMINATOR
/*Operators */
%token MINUS PLUS MULT DIV MOD 
%token XOR AND OR NOT
/* Logical operators */
%token LAND LOR LEQU
/* bracket and braces */
%token OBRACKET CBRACKET ARRINDO ARRINDC
%token USCORE
%token OBRACE CBRACE 

/*Built in functions */
%token PRINT

%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
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
	: VarDeclaration { printf("IT WORKED");/*$$ = $1*/ }
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
	| ParamaterDec ',' ParamListDec { /* $$ = param_vector.pushback($1)*/}
	;
ParamaterDec
	: Type Identifier {/**/}
	;

  /* Brackets Relational Operators*/
BitExp
	: BitExp AND Exp {}
	| BitExp OR Exp  {}
	| BitExp XOR Exp {}
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
	: Identifier WAS A Type {printf("Var Declared");/*add id to sym table*/}
	| Identifier HAD Identifier Type {/*arrays init*/}
	;

/* Array Initialisation/lval/rval, Result of functions*/
Assignment
	: Identifier BECAME BitExp   {/*update value in symbol table*/}
	| Identifier BECAME CHAR {/*also need to check errors*/}
	| Identifier BECAME ArrayVal
	;

ArrayVal
	: Identifier ARRINDO Value ARRINDC {/*value is an index*/}
	;
 
/*Print Token matches said allice and spoke*/
Print 
	: PRINT  {/**/}
	;

/* Add rules for Conditoinals/loop/ArrayAcess/FuncandProcedureCalls
 * user input / null statement / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator {printf("Variable Declared\n");} 
	| Read {}
	| Conditional {}
	| Loop {}
	| Call {}
/*	| ProcCall {}*/
	| ProcedureDec {}
	| FunctionDec {}
	| Null {}
	| Increment Separator {}
	| Decrement Separator {}
	| Codeblock {}
      /*| Generalise Print */
	| Assignment Separator {}
	| BitExp Print Separator {}
	| StringLit Print Separator {}
	;

StringLit
	: '\"' STRING '\"' 
	;


Call 
	: Identifier OBRACKET ParamList CBRACKET {}
	;
ParamList
	: Paramater ',' ParamList
	| Paramater
	;
Paramater
	: Call {}
	| Identifier {}
	| StringLit {}
	;
Read
	: WHATWAS Identifier QUESTIONMARK
	;
Loop
	: EVENTUALLY OBRACKET Predicate CBRACKET BECAUSE StatementList ENOUGHTIMES
	;
Null
	: "."
	;

Predicate
	: Exp LEQU Exp
	| Exp LOR Exp
	| Exp LAND Exp
	| OBRACKET Predicate CBRACKET
	;

Increment
	: Identifier INC
	;

Decrement
	: Identifier DEC
	;	
Conditional
	: PERHAPS OBRACKET Predicate CBRACKET SO Statement ORKEYWORD Statement UNSUREALICE
	| EITHER OBRACKET Predicate CBRACKET SO Statement ORKEYWORD Statement UNSUREALICE
	;
Codeblock
	: OBRACE StatementList CBRACE {/* create new scoping symtable + vector*/}
	; 

StatementList
	: StatementList Statement {}
	| Statement {}
	;

Separator
	: TERMINATOR {}
	| SEPARATOR { printf("WE GOT A SEPARATOR?");}
	;

Identifier
	: STRING {printf("New Identifier: %s\n", yyval.string);}
	/* CHAR {printf("we got a char\n");} */
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
