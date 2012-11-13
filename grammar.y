%{
#include <stdio.h>
#include <string.h>
%}

%token CHAR
%token STRING
%token INTEGER
%token SEPERATOR
%token TERMINATOR
%token MINUS
%token PLUS
%token MULT
%token DIV
%token MOD 
%token XOR
%token AND
%token OR
%token NOT
%token OBRACKET
%token CBRACKET
%token USCORE
%token OBRACE
%token CBRACE 

%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UNARY

%%

DeclarationList
	: Declaration DeclarationList { /*code here */ }
	| Declaration { /*$$ = $1; */ } 
	;

Declaration
	: VarDeclaration { /*$$ = $1*/ }
	| FunctionDec	 { /*$$ = $1*/ }
	| ProcedureDec     { /*$$ = $1*/ }
	;

FunctionDec
	: "The room" Identifier OBRACKET ParamListDec CBRACKET "contained a" Type Codeblock
	{/*$$ = new funcNode($2,$7,$8,$4) name/type/body/args*/}
	;

ProcedureDec
	:  "The looking-glass" Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ /*$$ = new procNode( $2,$6,$4) name/body/args */ }
	;

ParamListDec
	: ParamaterDec {/*create a std::vector */}
	| ParamaterDec ',' ParamListDec { /* $$ = param_vector.pushback($1)*/}
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
	: "number" {}
	| "letter" {}
	| "sentence" {}
	| "spider" Type {}
	;

VarDeclaration
	: Identifier "was a" Type {/*add id to sym table*/}
	| Identifier "had" Identifier Type {/*arrays init*/}
	;

/* Array Initialisation/lval/rval, Result of functions*/
Assignment
	: Identifier "became" LogExp   {/*update value in symbol table*/}
	| Identifier "became" CHAR	{/*also need to check errors*/}
	| Identifier "became" ArrayVal
	;

ArrayVal
	: Identifier '\'' 's' Value "piece" {/*value is an index*/}
	;
 
/* Clean up the 'print' grammar, to include spoke, also it only works with logExp*/
Print
	: "spoke" 	{/**/}
	| "said Alice"  {/**/}
	;

/* Add rules for Conditoinals/loop/ArrayAcess/FuncandProcedureCalls
 * user input / null statement / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator {}
	| Read {}
	| Conditional {}
	| Loop {}
	| FuncCall {}
/*	| ProcCall {}*/
	| ProcedureDec {}
	| FunctionDec {}
	| Null {}
	| Increment
	| Decrement
	| Codeblock {$$ = $1}
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
	: "eventually" OBRACKET Test CBRACKET "because" StatementList "enough times"
	;
Null
	: "."
	;

Test
	: Exp "==" Exp /*fill in rest later*/
	;

Increment
	: Identifier "ate"
	;

Decrement
	: Identifier "drank"
	;	
Conditional
	: "perhaps" OBRACKET Test CBRACKET "so" Statement "or" Statement "because Alice was unsure which"
	| "either" OBRACKET Test CBRACKET "so" Statement "or" Statement "because Alice was unsure which"
	;
Codeblock
	: OBRACE StatementList CBRACE {/* create new scoping symtable + vector*/}
	; 

StatementList
	: StatementList Statement {}
	| Statement {}
	;

Separator
	: ',' {}
	| TERMINATOR {}
	| "and" {}
	| "but" {}
	;

Identifier
	: STRING {}
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

