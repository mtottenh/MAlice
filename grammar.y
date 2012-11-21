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
%token<token> TCHAR TSTRING TREF TNUMBER 
/* Alice Keywords */
%token OF WAS PROCEDURE FUNC BECAME INC DEC CONTAINEDA HAD WHATWAS 
QUESTIONMARK EVENTUALLY BECAUSE ENOUGHTIMES THEN ELSE IF ENDIF MAYBE TOO
FOUND KEYWORD

/* Primitives */
%token <string> CHARLIT STRING STRINGLIT
%token <token> INTEGER
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
	int token; /* should we explicitly state the length? e.g. int_32t?*/
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
%type <node> VarDeclaration Return PredPrime ParamListDec Read Loop
%type <node> FunctionDec ProcedureDec ParamList Char StringLit
%type <node> Codeblock Conditional Predicate Maybe
%type <node> BitExp Exp Term Factor Value ArrayVal Call Increment Decrement
%type <assignment> Assignment 
%type <node> Statement
%type <id> Identifier ParameterDec
%type <token> Type
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

ParamListDec
	: ParameterDec {$$ = new NParamDeclarationBlock($1); }
	| ParamListDec COMMA ParameterDec {$1->children.push_back($3); }
	;
ParameterDec
	: Type Identifier {$$ = $2; }
	;

/* 
 * (TODO)Numerical Expressions are created using the NBinOp class which accepts:
 * pointer to the left subtree
 * an opperator token which is returned from the lexer.
 * a pointer to the right subtree
 */
BitExp
	: Exp AND BitExp {$$ = new NBinOp($1, $3, BAND);}
	| Exp OR BitExp  {$$ = new NBinOp($1, $3, BOR);}
	| Exp XOR BitExp {$$ = new NBinOp($1, $3, BXOR);}
	| Exp {$$ = $1;}
	;
Exp
	: Exp PLUS Term {$$ = new NBinOp($1, $3, BPLUS);}
	| Exp MINUS Term {$$ = new NBinOp($1, $3, BMINUS);}
	| Term {$$ = $1;}
	;
Term
	: Term MULT Factor {$$ = new NBinOp($1, $3, BMULT);}
	| Term DIV Factor {$$ = new NBinOp($1, $3, BDIV);}
	| Term MOD Factor {$$ = new NBinOp($1, $3, BMOD);}
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
 * NOTE -> The behaviour of nodes that use the above two 
 * should probably get the value from the node and delete it 
 * rather than storing it as a child.
 * Other options: - we use the relevant subrules to create
 * Nodes
 */
Value
	: INTEGER {$$ = new NInteger($1);}
	| Identifier {$$ = $1;}
	| Call { $$ = $1;}
	| ArrayVal {$$ = $1;}
	| OBRACKET BitExp CBRACKET { $$ = $2;}
	;

/* 
 * (TODO) Types: here we should just return an integer relating to the token 
 * returned by the lexer
 */
Type
	: TNUMBER { $$ = $1; }
	| TCHAR { $$ = $1; }
	| TSTRING {$$ = $1;}
 /* This is actually passing by reference and not a PTR type */
	| TREF Type {}
	;
/*
 * (TODO) To create a declaration node we pass it an ID node and its type
 * we should also add it to the symbol table.
ar.y:141:56: error: no matching function for call to
‘NIdentifier::NIdentifier(NIdentifier*&)’
* for array declarations we add an extra paramater, ie the number of elements
 */
VarDeclaration
	: Identifier WAS Type {$$ = new NVariableDeclaration($1,$3);/*add id to sym table*/}
	| Identifier HAD BitExp Type {$$ = new NVariableDeclaration($1,$4,$3); }
	;
/*
 * (TODO) Assignment Nodes - Should accept:
 * An Identifier Node which is just uses to extract the value then deletes
 * An rval Node pointer 
 * Need to add cases for assigning things to stringlit's 
 */
Assignment
	: Identifier BECAME BitExp   { $$ = new NAssignment($1,$3);}
	| Identifier BECAME Char {$$ = new NAssignment($1,$3);/*need to check errors*/}
	| ArrayVal BECAME BitExp {$$ = new NAssignment($1,$3); }
	| ArrayVal BECAME Char {$$ = new NAssignment($1,$3);}
	;
/*
 * (TODO)This should return a StatementList Node with two sub nodes
 * one a  declaration node ($1)
 * and one an assignment node (new NStatementList(dec,ass))
 * need to overload assignment constructor to accept a string aswell as 
 * a pointer to an ID node
 */
VarDeclarationAssignment
	: VarDeclaration OF BitExp 
	{ NVariableDeclaration* Declaration = (NVariableDeclaration *)$1; 
	  Node* Assignment =  new NAssignment(Declaration->getID(), $3);
	  $$ = new NStatementList(Declaration,Assignment);}
	| VarDeclaration OF Char 
	{ NVariableDeclaration* Declaration = (NVariableDeclaration *)$1; 
	  Node* Assignment =  new NAssignment(Declaration->getID(), $3);
	  $$ = new NStatementList(Declaration,Assignment);}
	| VarDeclaration OF StringLit 
	{ NVariableDeclaration* Declaration = (NVariableDeclaration *)$1; 
	  Node* Assignment =  new NAssignment(Declaration->getID(), $3);
	  $$ = new NStatementList(Declaration,Assignment);}
	;

/* (TODO) change value to be bit exp?
 * The NArray class takes two arguments
 * the first is just gets the identifier value  and deletes the node 
 * the second it stores as a child to be evaluated later
 */
ArrayVal
	: Identifier ARRINDO Value ARRINDC 
	{$$ = new NArray($1,$3); }
	;
 
/*Print Token matches said allice and spoke*/
//Print 
//	: PRINT  {$$ = new NPrint();/**/}
//	;

/*
 * (TODO) Make a return node and
 * pass it an expression to return;
 */
Return
	: FOUND BitExp {$$ = new NReturn($2);}
	;

/* Add rules for /ArrayAcess/FuncandProcedureCalls
 * user input / fix LoxExp print to be more generatlised 
 *  */
Statement
	: VarDeclaration Separator {$$ = $1;}
	| VarDeclaration TOO Separator {$$ = $1;} 
	| VarDeclarationAssignment Separator {$$ = $1;}
	| Read {$$=$1;}
	| Conditional { $$ = $1;}
	| Loop {$$=$1;}
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
	| StringLit PRINT Separator {$$ = new NPrint($1);}
	| Char PRINT Separator { $$ = new NPrint($1);}
	| Return Separator {$$ = $1;}
	;

Call 
	: Identifier OBRACKET ParamList CBRACKET {$$ = new NMethodCall($1,$3);}
	| Identifier OBRACKET CBRACKET {$$ = new NMethodCall($1);}
	;
/* (TODO)
 */
ParamList
	: Paramater COMMA ParamList {}
	| Paramater {}
	;
Paramater
	: StringLit {}
	| Char {}
	| BitExp {}
	;
Read
	: WHATWAS Identifier QUESTIONMARK {}
	;
Loop
	: EVENTUALLY OBRACKET Predicate CBRACKET BECAUSE StatementList 
		ENOUGHTIMES { $$ = new NLoop($3, $6); }
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

/* We need to change the behaviour of these constructors 
 * to extract $1's value with getID() and then 
 * delete the node, will make check() easier
 */
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
	: STRING {$$ = new NIdentifier($1);}
	;
StringLit
	: STRINGLIT {$$ = new NStringLit($1);}
Char
	: CHARLIT { $$ = new NChar($1); }
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
