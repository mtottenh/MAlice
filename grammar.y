%{
#include <stdio.h>
#include <string.h>
extern int yyerror();
%}

/* Type Tokens */
%token TCHAR TSTRING TPTR TNUMBER  
/* Alice Keywords */
%token WAS A PROCEDURE FUNC BECAME INC DEC	

/* Primitives */
%token CHAR STRING INTEGER
%token SEPARATOR TERMINATOR
/*Operators */
%token MINUS PLUS MULT DIV MOD 
%token XOR AND OR NOT
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
	| FunctionDec	 { /*$$ = $1*/ }
	| ProcedureDec     { /*$$ = $1*/ }
	;

FunctionDec
	: FUNC Identifier OBRACKET ParamListDec CBRACKET "contained a" Type Codeblock
	{/*$$ = new funcNode($2,$7,$8,$4) name/type/body/args*/}
	;

ProcedureDec
	:  PROCEDURE Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ /*$$ = new procNode( $2,$6,$4) name/body/args */ }
	;

ParamListDec
	: ParamaterDec {/*create a std::vector */}
	| ParamaterDec ',' ParamListDec { /* $$ = param_vector.pushback($1)*/}
	| ;
	;
ParamaterDec
	: Type Identifier {/**/}
	;

/* Brackets Relational Operators
*/
LogExp
	: LogExp AND Exp {}
	| LogExp OR Exp  {}
	| LogExp XOR Exp {}
	| OBRACKET LogExp CBRACKET {}
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
	| Identifier "had" Identifier Type {/*arrays init*/}
	;

/* Array Initialisation/lval/rval, Result of functions*/
Assignment
	: Identifier BECAME LogExp   {/*update value in symbol table*/}
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
	| FuncCall {}
/*	| ProcCall {}*/
	| ProcedureDec {}
	| FunctionDec {}
	| Null {}
	| Increment Separator {}
	| Decrement Separator {}
	| Codeblock {}
      /*| Generalise Print */
	| Assignment Separator {}
	| LogExp Print Separator {}
	| StringLit Print Separator {}
	;

StringLit
	: '\"' STRING '\"' 
	;


FuncCall 
	: Identifier OBRACKET ParamList CBRACKET {}
	;
ParamList
	: Paramater ',' ParamList
	| Paramater
	;
Paramater
	: FuncCall {}
	| Identifier {}
	| StringLit {}
	;
Read
	: "what was" Identifier "?"
	;
Loop
	: "eventually" OBRACKET Predicate CBRACKET "because" StatementList "enough times"
	;
Null
	: "."
	;

Predicate
	: Exp "==" Exp /*fill in rest later*/
	| Exp "||" Exp
	| Exp "&&" Exp
	| OBRACKET Predicate CBRACKET
	;

Increment
	: Identifier INC
	;

Decrement
	: Identifier DEC
	;	
Conditional
	: "perhaps" OBRACKET Predicate CBRACKET "so" Statement "or" Statement "because Alice was unsure which"
	| "either" OBRACKET Predicate CBRACKET "so" Statement "or" Statement "because Alice was unsure which"
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
	: STRING {printf("New Identifier: %s\n", yyval);}
	/* CHAR {printf("we got a char\n");} */
	;

%%
main()
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
