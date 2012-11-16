%{
#include <string>
#include <iostream>
#include "Node.hpp"
#include "TreePrinter.hpp"
extern void yyerror(char*);
extern int yylex();
Node *root;
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
%start program 

%left UNARY
%union {
	char *string;
	int token;
	Node *node;
	NCodeBlock *block;
	NExpression *exp;
	NPrint *print;
	NStatement *stat;
	NIdentifier *id;
	NAssignment *assignment;
	NVariableDeclaration *var_dec;
	NFunctionDeclaration *func_dec;
	NDeclarationBlock *dec_list;
	std::vector<NVariableDeclaration *> *paramlist;
	std::vector<NStatement *> *statlist;
	
}

%type <dec_list> DeclarationList Declaration program
%type <var_dec> VarDeclaration
%type <func_dec> FunctionDec ProcedureDec
%type <block> Codeblock
%type <exp> BitExp Exp Term Factor Value 
%type <assignment> Assignment
%type <stat> Statement
%type <print> Print
%type <id> Identifier
%type <stat> StatementList
%%

program : DeclarationList { root = $1; }
	;

DeclarationList
	: Declaration DeclarationList { $2->children.push_back($1);
					/*code here */ }
	| Declaration { $$ = new NDeclarationBlock(); $$->children.push_back($1);} 
	;

Declaration
	: VarDeclaration Separator { $$->children.push_back($1) ; }
	| VarDeclarationAssignment Separator {}
	| FunctionDec	 { $$->children.push_back($1); }
	| ProcedureDec     { $$->children.push_back($1);  printf("IT WORKED?");}
	;

FunctionDec
	: FUNC Identifier OBRACKET ParamListDec CBRACKET CONTAINEDA Type Codeblock
	{$$ = new NFunctionDeclaration(); $$->children.push_back($8);}
	| FUNC Identifier OBRACKET CBRACKET CONTAINEDA Type Codeblock {$$ = new NFunctionDeclaration(); $$->children.push_back($7);}
	;

ProcedureDec
	:  PROCEDURE Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ $$ = new NFunctionDeclaration(); $$->children.push_back($6);/*$$ = new procNode( $2,$6,$4) name/body/args */ }
	|  PROCEDURE Identifier OBRACKET CBRACKET Codeblock {$$ = new NFunctionDeclaration();printf("before push"); $$->children.push_back($5); printf("It worked :(");}
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
	| Exp {$$ = $1;}
	;
Exp
	: Exp PLUS Term {$$ = new NBinOP();}
	| Exp MINUS Term {$$ = new NBinOP();}
	| Term {$$ = $1;}
	;
Term
	: Term MULT Factor {}
	| Term DIV Factor {}
	| Term MOD Factor {}
	| Factor {$$ = $1;}
	;
Factor
	: NOT Value %prec UNARY {}
	| MINUS Value %prec UNARY {}
	| Value {$$ = $1;}
	;
Value
	: INTEGER {$$ = new NInteger();}
	| Identifier {$$ = new NIdentifier();}
<<<<<<< HEAD
	| Call {}
	| ArrayVal {}
=======
>>>>>>> Made significant changes to grammar in order to support AST creation
	;
/* Add Array/String type */
Type
	: TNUMBER {}
	| TCHAR {}
	| TSTRING {}
	| TPTR Type {}
	;

VarDeclaration
	: Identifier WAS A Type {$$ = new NVariableDeclaration();/*add id to sym table*/}
	| Identifier HAD BitExp Type {printf("Array Declared\n"); }
	;

Assignment
	: Identifier BECAME BitExp   { $$ = new NAssignment();/*update value in symbol table*/}
	| Identifier BECAME CHAR {/*also need to check errors*/}
	| Identifier BECAME ArrayVal {}
	| ArrayVal BECAME BitExp {}
	| ArrayVal BECAME CHAR {}
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
	: PRINT  {$$ = new NPrint();/**/}
	;

Return
	: FOUND BitExp
	;

/* Add rules for /ArrayAcess/FuncandProcedureCalls
 * user input / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator {$$ = $1;} 
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
	| Assignment Separator {$$ = $1;}
	| BitExp Print Separator {$$ = $2;}
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
	: IF OBRACKET Predicate CBRACKET THEN Statement Maybe
	;

Maybe
	: ELSE Statement ENDIF
	| ENDIF
	| ELSE MAYBE OBRACKET Predicate CBRACKET THEN Statement Maybe
	;

Codeblock
	: OBRACE StatementList CBRACE {$$ = new NCodeBlock(); $$->children.push_back($2);/* create new scoping symtable + vector*/}
	; 

StatementList
	: StatementList Statement {$1->children.push_back($2);}
	| Statement{$$ = new NStatement(); $$->children.push_back($1);}
	;

Separator
	: NULLTOK {}
	| SEPARATOR {}
	| COMMA {}
	;

Identifier
	: STRING {$$ = new NIdentifier();}
	;

%%
int main()
{
 int node = yyparse();
 printf(" WE HAVENT SETFAULTED YET");
 treePrinter t(root);
 t.print(); 
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
