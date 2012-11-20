%{
#include <string>
#include <iostream>
#include "Node/Node.hpp"
#include "Node/NodeIncludes.hpp"
#include "TreePrinter.hpp"
extern void yyerror(char*);
extern int yylex();
Node *root;
%}

/* Type Tokens */
%token TCHAR TSTRING TPTR TNUMBER  
/* Alice Keywords */
%token OF WAS PROCEDURE FUNC BECAME INC DEC CONTAINEDA HAD WHATWAS 
QUESTIONMARK EVENTUALLY BECAUSE ENOUGHTIMES THEN ELSE IF ENDIF MAYBE TOO
FOUND

/* Primitives */
%token <string> CHAR STRING INTEGER STRINGLIT
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
	NStatementList *stat;
	NIdentifier *id;
	NAssignment *assignment;
	NVariableDeclaration *var_dec;
	NFunctionDeclaration *func_dec;
	NDeclarationBlock *dec_list;
	std::vector<NVariableDeclaration *> *paramlist;
	
}

%type <node> DeclarationList Declaration program VarDeclarationAssignment
%type <node> VarDeclaration Return PredPrime 
%type <node> FunctionDec ProcedureDec
%type <node> Codeblock Conditional Predicate Maybe
%type <node> BitExp Exp Term Factor Value ArrayVal Call Increment Decrement
%type <assignment> Assignment 
%type <node> Statement
%type <id> Identifier
%type <stat> StatementList
/* UNDCIDED ONES LOL */
 /*%type <stat> */
%%

/* (TODO) A Note On the symbol table and Type resolution:
 * As our symbol table stores pointers to nodes in the AST as opposed to 
 * type informatino for identifiers. The nodes int the AST MUST provide
 * a method of resolving types of identifiers. This can be achived
 * by adding an overideable method in the base class (Node) called
 * getType(); We will need to also make sure that the constructors
 * of our nodes is where the type resolution takes place.
 * if we cannot resolve a type, it is probably prudent to create a default 
 * 'undefined' type so that the rest of the AST's symantic checking 
 * can continue
 */

/* We define a program as a list of Declarations, this allows global vaiables*/

program : DeclarationList { root = $1; }
	;

DeclarationList
	: DeclarationList Declaration 
	{ $1->children.push_back($2); }
	| Declaration 
	{ $$ = new NDeclarationBlock($1); } 
	;
/*
 * The value of a declaration node is just defined as the 
 * evaluation of one of the possible things it declares
 * i.e. a generic Declaration node should not appear int the tree
 */
Declaration
	: VarDeclaration Separator { $$=$1 ; }
	| VarDeclaration TOO Separator {$$ = $1;}
	| VarDeclarationAssignment Separator { $$ = $1;}
	| FunctionDec	 { $$ = $1; }
	| ProcedureDec     { $$= $1;  }
	;

/* 
 * To declare a function we create a function node and pass it the following:
 * Its Identifier - which we add to the symbol table
 * (OPTIONAL) A pointer to a list of its paramaters (as a subtree in the AST)
 * its return type
 * A pointer to a Codeblock which contains the functions implementation
 * The function should try to infer its type from the paramters and return type
 */
FunctionDec
	: FUNC Identifier OBRACKET ParamListDec CBRACKET CONTAINEDA Type Codeblock
	{$$ = new NFunctionDeclaration($2,$4,$8,$7);}
	| FUNC Identifier OBRACKET CBRACKET CONTAINEDA Type Codeblock 
	{$$ = new NFunctionDeclaration($2,$7,$6);}
	;

/* 
 * To declare a procedure we create a function node and pass it the following:
 * Its Identifier - which we add to the symbol table
 * (OPTIONAL) A pointer to a list of its paramaters (as a subtree in the AST)
 * A pointer to a Codeblock which contains the functions implementation
 * The return type of a procedure should be undefined 
 */
ProcedureDec
	:  PROCEDURE Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ $$ = new NFunctionDeclaration($2,$4,$6);
	/*$$ = new procNode( $2,$6,$4) name/body/args */ }
	|  PROCEDURE Identifier OBRACKET CBRACKET Codeblock 
	{$$ = new NFunctionDeclaration($2,$5);/* $$->children.push_back($5);*/}
	;

/* TODO - we need to create classes for these */
ParamListDec
	: ParamaterDec {/*create a std::vector */}
	| ParamaterDec COMMA ParamListDec { /* $$ = param_vector.pushback($1)*/}
	;
ParamaterDec
	: Type Identifier {/**/}
	;

/* 
 * (TODO)Numerical Expressions are created using the NBinOp class which accepts:
 * pointer to the left subtree
 * an opperator token which is returned from the lexer.
 * a pointer to the right subtree
 */
BitExp
	: Exp AND BitExp {$$ = new NBinOp();}
	| Exp OR BitExp  {$$ = new NBinOp();}
	| Exp XOR BitExp {$$ = new NBinOp();}
	| Exp {$$ = $1;}
	;
Exp
	: Exp PLUS Term {$$ = new NBinOp();}
	| Exp MINUS Term {$$ = new NBinOp();}
	| Term {$$ = $1;}
	;
Term
	: Term MULT Factor {$$ = new NBinOp();}
	| Term DIV Factor {$$ = new NBinOp();}
	| Term MOD Factor {$$ = new NBinOp();}
	| Factor {$$ = $1;}
	;
/*
 * Unary Operators are created with the NUnaryOp class which accepts
 * the operator token returned from the lexer
 * a pointer to the subexpression to apply to operator too
 */
Factor
	: NOT Factor %prec UNARY {$$ = new NUnaryOp($1,$2);}
	| MINUS Factor %prec UNARY {$$ = new NUnaryOp($1,$2);}
	| Value { $$ = $1;}
	;
/*
 * (TODO)To create values we have several different options:
 * Integers: - we just create an integer node with the 
 * valude that the lexer returns as a paramter (CHECK IN LEXER)
 * Identifier: - We create an NIdentifier node and pass it the 
 * Name of the identifier returned from the lexer.
 * Other options: - we use the relevant subrules to create
 * Nodes
 */
Value
	: INTEGER {$$ = new NInteger($1);}
	| Identifier {$$ = new NIdentifier();}
	| Call { $$ = $1;}
	| ArrayVal {$$ = $1;}
	| OBRACKET BitExp CBRACKET { $$ = $2;}
	;

/* Add Array/String type */
Type
	: TNUMBER {}
	| TCHAR {}
	| TSTRING {}
	| TPTR Type {}
	;

VarDeclaration
	: Identifier WAS Type {$$ = new NVariableDeclaration();/*add id to sym table*/}
	| Identifier HAD BitExp Type {printf("Array Declared\n"); }
	;

Assignment
	: Identifier BECAME BitExp   { $$ = new NAssignment();/*update value in symbol table*/}
	| Identifier BECAME CHAR {/*also need to check errors*/}
	| ArrayVal BECAME BitExp {}
	| ArrayVal BECAME CHAR {}
	;

VarDeclarationAssignment
	: VarDeclaration OF BitExp {$$ = $1;}
	| VarDeclaration OF CHAR	{printf("and assigned\n");}
	| VarDeclaration OF STRINGLIT {printf("and assigned\n");}
	;

/* change value to be bit exp */
ArrayVal
	: Identifier ARRINDO Value ARRINDC {/*value is an index*/}
	;
 
/*Print Token matches said allice and spoke*/
//Print 
//	: PRINT  {$$ = new NPrint();/**/}
//	;
/* should probably make a return node*/
Return
	: FOUND BitExp {$$ = $2;}
	;

/* Add rules for /ArrayAcess/FuncandProcedureCalls
 * user input / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator {$$ = $1;}
	| VarDeclaration TOO Separator {$$ = $1;} 
	| VarDeclarationAssignment Separator {$$ = $1;}
	| Read {}
	| Conditional { $$ = $1;}
	| Loop {}
	| Call Separator { $$ = $1;}
	| ProcedureDec {$$ = $1;}
	| FunctionDec {$$ = $1;}
	| NULLTOK {}
	| Increment Separator {$$ = $1;}
	| Decrement Separator {$$ = $1;}
	| Codeblock {$$ = $1;}
      /*| Generalise Print */
	| Assignment Separator {$$ = $1;}
	| BitExp PRINT Separator {$$ = new NPrint($1);}
	| STRINGLIT PRINT Separator {$$ = new NPrint($1);}
	| CHAR PRINT Separator { $$ = new NPrint($1);}
	| Return Separator {$$ = $1;}
	;

Call 
	: Identifier OBRACKET ParamList CBRACKET {$$ = new NMethodCall();}
	| Identifier OBRACKET CBRACKET {$$ = new NMethodCall();}
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
	: PredPrime LOR Predicate {$$ = new NPredicate($1,$2,$3);}
	| PredPrime LAND Predicate {$$ = new NPredicate($1,$2,$3);}
	| PredPrime {$$ = $1;}
	| LNOT OBRACKET Predicate CBRACKET {$$ = new NPredicate($1,$3);}
	| OBRACKET Predicate CBRACKET {$$ = $2;}
	;

PredPrime
	: BitExp LEQU BitExp  {$$ = new NPredicate($1,$2,$3);}
	| BitExp LLTHAN BitExp {$$ = new NPredicate($1,$2,$3);}
	| BitExp LLTHANEQ BitExp  {$$ = new NPredicate($1,$2,$3);}
	| BitExp LGTHAN BitExp  {$$ = new NPredicate($1,$2,$3);}
	| BitExp LGTHANEQ BitExp  {$$ = new NPredicate($1,$2,$3);}
	;

Increment
	: Identifier INC { $$ = new NInc($1);}
	;

Decrement
	: Identifier DEC { $$ = new NDec($1);}
	;	

Conditional
	: IF OBRACKET Predicate CBRACKET THEN StatementList Maybe
	 { $$ = new NConditional($3,$6,$7);}
	;

Maybe
	: ELSE StatementList ENDIF {$$ = $2;}
	| ENDIF {}
	| ELSE MAYBE OBRACKET Predicate CBRACKET THEN StatementList Maybe 
	{$$ = new NConditional($4,$7,$8);}
	;

Codeblock
	: OBRACE StatementList CBRACE 
	{$$ = $2;}
/* Old code, I think we dont really need to make a new scope block 
 * as statlist seems to create the same functionality in the tree,
 * IE we can combine these rules to prune the AST
 */
  /*	{$$ = new NCodeBlock(); $$->children.push_back($2);} */
	
	/* Still need to create new scoping symtable + vector*/ 
	; 

StatementList
	: StatementList Statement {$1->children.push_back($2);}
	| Statement{$$ = new NStatementList(); $$->children.push_back($1);}
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
