%{
#include <string>
#include <iostream>
#include "Node/NodeIncludes.hpp"
#include "TreePrinter/TreePrinter.hpp"
#include "Errors/TypeMap.hpp"

extern void yyerror(char*);
extern int yylex();
Node *root;
%}


/* Type Tokens */
%token<token> TCHAR TSTRING TREF TNUMBER REFCHAR REFSTRING REFNUMBER
/* Alice Keywords */
%token OF WAS PROCEDURE FUNC BECAME INC DEC CONTAINEDA HAD WHATWAS 
QUESTIONMARK EVENTUALLY BECAUSE ENOUGHTIMES THEN ELSE IF ENDIF MAYBE TOO
FOUND VARDEC PARAMDEC

/* Extra 'types' for semantic analysis (add to types enum). */
%token INVALIDTYPE BOOLEAN VOID 

/* Primitives */
%token <string> CHARLIT STRING STRINGLIT
%token <token> INTEGER
%token SEPARATOR NULLTOK COMMA QUOTE
/*Operators */
%token <token> MINUS PLUS MULT DIV MOD 
%token <token> XOR AND OR NOT
/* Logical operators */
%token <token> LAND LOR LEQU LNOT LNOTEQU LGTHAN LGTHANEQ LLTHAN LLTHANEQ
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
	NPrint *print;
	NStatementList *stat;
	NIdentifier *id;
	NAssignment *assignment;
	NVariableDeclaration *var_dec;
	NFunctionDeclaration *func_dec;
	NDeclarationBlock *dec_list;
}

%type <node> DeclarationList Declaration program ParameterDec VarDeclarationAssignment
%type <node> VarDeclaration Return PredPrime ParamListDec ParamList
%type <node> Loop
%type <func_dec> FunctionDec ProcedureDec
%type <node> Codeblock Conditional Predicate Maybe
%type <node> BitExp Exp Term Factor Value ArrayVal Call 
%type <assignment> Assignment Read
%type <node> Statement
%type <node> StringLit Char Parameter
%type <id> Identifier Increment Decrement
%type <token> Type
%type <stat> StatementList
/* UNDCIDED ONES LOL */
 /*%type <stat> */
%%

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
 * (TODO) Look into TREF
 */
Type
	: TNUMBER { $$ = $1; }
	| TCHAR { $$ = $1; }
	| TSTRING {$$ = $1;}
 /* We need to get the type 'number' rather than value etc. */
	| TREF TCHAR {$$ = REFCHAR;}
	| TREF TNUMBER {$$ = REFNUMBER;}
	| TREF TSTRING {$$ = REFSTRING;}
	;
VarDeclaration
	: Identifier WAS Type {$$ = new NVariableDeclaration($1,$3);}
	| Identifier HAD BitExp Type {$$ = new NVariableDeclaration($1,$4,$3); }
	;

FunctionDec
	: FUNC Identifier OBRACKET ParamListDec CBRACKET CONTAINEDA Type Codeblock
	{$$ = new NFunctionDeclaration($2,$4,$8,$7);}
	| FUNC Identifier OBRACKET CBRACKET CONTAINEDA Type Codeblock 
	{$$ = new NFunctionDeclaration($2,$7,$6);}
	;

ProcedureDec
	:  PROCEDURE Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ $$ = new NFunctionDeclaration($2,$4,$6);
	/*$$ = new procNode( $2,$6,$4) name/body/args */ }
	|  PROCEDURE Identifier OBRACKET CBRACKET Codeblock 
	{$$ = new NFunctionDeclaration($2,$5);/* $$->children.push_back($5);*/}
	;
/* WE NEED TO EDIT THIS TO ADD TYPE INFORMATION TO THE CONSTRUCTOR!" */
ParamListDec
	: ParameterDec {$$ = new NParamDeclarationBlock($1); }
	| ParamListDec COMMA ParameterDec {$1->children.push_back($3); }
	;
ParameterDec
	: Type Identifier {$$ = new NVariableDeclaration($2,$1); }
	;

BitExp
	: BitExp AND Exp {$$ = new NBinOp($1, $3, BAND);}
	| BitExp OR Exp {$$ = new NBinOp($1, $3, BOR);}
	| BitExp XOR Exp {$$ = new NBinOp($1, $3, BXOR);}
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
Value
	: INTEGER {$$ = new NInteger($1);}
	| Identifier {$$ = $1;}
	| Call { $$ = $1;}
	| ArrayVal {$$ = $1;}
	| Increment {$$ = $1;}
	| Decrement {$$ = $1;}
	| OBRACKET BitExp CBRACKET { $$ = $2;}
	;
//Check string lit case!
Assignment
	: Identifier BECAME BitExp   { $$ = new NAssignment($1,$3);}
	| Identifier BECAME Char {$$ = new NAssignment($1,$3);/*need to check errors*/}
	| ArrayVal BECAME BitExp {$$ = new NAssignment($1,$3); }
	| ArrayVal BECAME Char {$$ = new NAssignment($1,$3);}
	;
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

/*Print Token matches said allice and spoke*/
//Print 
//	: PRINT  {$$ = new NPrint();/**/}
//	;

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
	| NULLTOK {$$ = new NNullToken();}
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
ParamList
	: ParamList COMMA Parameter { $1->children.push_back($3); }
	| Parameter { $$ = new NParamBlock($1); }
	;
Parameter
	: StringLit {$$ = $1;}
	| Char {$$ = $1;}
	| BitExp {$$ = $1;}
	;
Read
	: WHATWAS Identifier QUESTIONMARK { $$ = new NAssignment($2, 
										new NInput()); }
	| WHATWAS ArrayVal QUESTIONMARK {$$ = new NAssignment($2, new NInput());}
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
	| BitExp LNOTEQU BitExp {$$ = new NPredicate($1,$2,$3);}
	;

Increment
	: Identifier INC { $$ = new NInc($1);}
	| ArrayVal INC {$$ = new NInc($1);}
	;

Decrement
	: Identifier DEC { $$ = new NDec($1);}
	| ArrayVal DEC {$$ = new NDec($1);}
	;	

Conditional
	: IF OBRACKET Predicate CBRACKET THEN StatementList Maybe
	 { $$ = new NConditional($3,$6,$7);}
	;

Maybe
	: ELSE StatementList ENDIF {$$ = $2;}
	| ENDIF {$$ = new NEndIf();}
	| ELSE MAYBE OBRACKET Predicate CBRACKET THEN StatementList Maybe 
	{$$ = new NConditional($4,$7,$8);}
	;

Codeblock
	: OBRACE StatementList CBRACE 
	{$$ = $2;}
	| OBRACE CBRACE {$$ = new NStatementList();}
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

ArrayVal
	: Identifier ARRINDO BitExp ARRINDC 
	{$$ = new NArrayAccess($1,$3); }
	;
 
Identifier
	: STRING {$$ = new NIdentifier($1);}
	;
StringLit
	: STRINGLIT {$$ = new NStringLit($1);}
Char
	: CHARLIT { $$ = new NCharLit($1); }
	;
%%

int initTypeMap();

int main()
{
 initTypeMap();
 int node = yyparse();
 treePrinter t(root);
 t.print(); 
 return node;
}

int initTypeMap() { 
	typemap_add(TCHAR, "letter");
	typemap_add(TSTRING, "sentence");
	typemap_add(TNUMBER, "number");
	typemap_add(REFCHAR, "spider letter");
	typemap_add(REFSTRING, "spider sentence");
	typemap_add(REFNUMBER, "spider number");
	typemap_add(INVALIDTYPE, "INVALID");
	return 1;
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
