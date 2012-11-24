%{
#include <string>
#include <iostream>
#include "Node/NodeIncludes.hpp"
#include "TreePrinter/TreePrinter.hpp"
#include "Errors/TypeMap.hpp"
#include "TreeWalker/SymbolTableGenerator.hpp"
extern void yyerror(char*);
extern int yylex();
Node *root;
%}


/* Type Tokens */
%token<token> TCHAR TSTRING TREF TNUMBER REFCHAR REFSTRING REFNUMBER
/* Alice Keywords */
%token FUNC PROCEDURE OF WAS A LOOKING GLASS BECAME INC DEC CONTAINED 
HAD WHAT QUESTIONMARK EVENTUALLY ENOUGH TIMES THEN ELSE IF MAYBE TOO
ALICE FOUND THE ROOM VARDEC PARAMDEC WHICH UNSURE BECAUSE SAID SPOKE CODEBLOCK
DECLARATIONBLOCK STATLIST

/* Extra 'types' for semantic analysis (add to types enum). */
%token INVALIDTYPE BOOLEAN VOID ENDIF

/* Primitives */
%token <string> CHARLIT STRING STRINGLIT
%token <token> INTEGER
%token SEPARATOR NULLTOK COMMA QUOTE
/*Operators */
%token <token> DASH PLUS MULT DIV MOD 
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
	NCodeBlock *code_block;
}

%type <node> Declaration program ParameterDec VarDeclarationAssignment
%type <node> VarDeclaration Return PredPrime ParamListDec ParamList
%type <node> Loop
%type <func_dec> FunctionDec ProcedureDec
%type <node> Conditional Predicate Maybe
%type <node> BitExp Exp Term Factor Value ArrayVal Call 
%type <assignment> Assignment Read
%type <node> Statement
%type <node> StringLit Char Parameter
%type <id> Identifier Increment Decrement
%type <token> Type
%type <stat> StatementList
%type <dec_list> DeclarationList
%type <code_block> Codeblock
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
	{ $$ = new NDeclarationBlock(); $$->children.push_back($1); } 
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
	: Identifier WAS A Type {$$ = new NVariableDeclaration($1,$4);}
	| Identifier HAD BitExp Type {$$ = new NVariableDeclaration($1,$4,$3); }
	;

FunctionDec
	: Func Identifier OBRACKET ParamListDec CBRACKET CONTAINED A Type Codeblock
	{$$ = new NFunctionDeclaration($2,$4,$9,$8);}
	| Func Identifier OBRACKET CBRACKET CONTAINED A Type Codeblock 
	{$$ = new NFunctionDeclaration($2,$8,$7);}
	;

ProcedureDec
	:  Procedure Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ $$ = new NFunctionDeclaration($2,$4,$6);
	/*$$ = new procNode( $2,$6,$4) name/body/args */ }
	|  Procedure Identifier OBRACKET CBRACKET Codeblock 
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
	: BitExp AND Exp {$$ = new NBinOp($1, $3, AND);}
	| BitExp OR Exp {$$ = new NBinOp($1, $3, OR);}
	| BitExp XOR Exp {$$ = new NBinOp($1, $3, XOR);}
	| Exp {$$ = $1;}
	;
Exp
	: Exp PLUS Term {$$ = new NBinOp($1, $3, PLUS);}
	| Exp DASH Term {$$ = new NBinOp($1, $3, DASH);}
	| Term {$$ = $1;}
	;
Term
	: Term MULT Factor {$$ = new NBinOp($1, $3, MULT);}
	| Term DIV Factor {$$ = new NBinOp($1, $3, DIV);}
	| Term MOD Factor {$$ = new NBinOp($1, $3, MOD);}
	| Factor {$$ = $1;}
	;
/*
 * Unary Operators are created with the NUnaryOp class which accepts
 * the operator token returned from the lexer
 * a pointer to the subexpression to apply to operator too
 */
Factor
	: NOT Factor %prec UNARY {$$ = new NUnaryOp($1,$2);}
	| DASH Factor %prec UNARY {$$ = new NUnaryOp($1,$2);}
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

Print
	: SAID ALICE {}
	| SPOKE {}
	;

Return
	: Found BitExp {$$ = new NReturn($2);}
	;

Found
	: ALICE FOUND {}
	;

/* Add rules for /ArrayAcess/FuncandProcedureCalls
 * user input / fix LoxExp print to be more generatlised 
 *  */
Statement
	: Read {$$=$1;}
	| Conditional { $$ = $1;}
	| Loop {$$=$1;}
	| Call Separator { $$ = $1;}
	| NULLTOK {$$ = new NNullToken();}
	| Increment Separator {$$ = $1;}
	| Decrement Separator {$$ = $1;}
	| Codeblock {$$ = $1;}
	| Assignment Separator {$$ = $1;}
	| BitExp Print Separator {$$ = new NPrint($1);}
	| StringLit Print Separator {$$ = new NPrint($1);}
	| Char Print Separator { $$ = new NPrint($1);}
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
	: WHAT WAS Identifier QUESTIONMARK { $$ = new NAssignment($3, 
										new NInput()); }
	| WHAT WAS ArrayVal QUESTIONMARK {$$ = new NAssignment($3, new NInput());}
	;
Loop
	: EVENTUALLY OBRACKET Predicate CBRACKET BECAUSE StatementList 
		ENOUGH TIMES { $$ = new NLoop($3, $6); }
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
	: ELSE StatementList EndIf {$$ = $2;}
	| EndIf {$$ = new NEndIf();}
	| ELSE MAYBE OBRACKET Predicate CBRACKET THEN StatementList Maybe 
	{$$ = new NConditional($4,$7,$8);}
	;

Codeblock
	: OBRACE DeclarationList StatementList CBRACE 
	{ $$ = new NCodeBlock($2, $3);}
	| OBRACE StatementList CBRACE { $$ = new NCodeBlock($2);}
	| OBRACE CBRACE {$$ = new NCodeBlock(); }
	| OBRACE DeclarationList CBRACE { $$ = new NCodeBlock($2);}
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
EndIf
	: BECAUSE ALICE WAS UNSURE WHICH {}
	;
Func
	: THE ROOM {}
	;
Procedure
	: THE LOOKING DASH GLASS {}
	;
%%

int initTypeMap();
extern FILE * yyin;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "ERROR: Usage is: " << argv[0] << " FILENAME" << endl;
		return 0;
	}

	/* Open file from argv[1]. Quit if null. */
	FILE *input = fopen(argv[1],"r");
	if (input == NULL) {
 		cout << "ERROR: Could not open file " << argv[1] << endl;
 		return 0;
	}

	yyin = input;
	initTypeMap();
	int node = yyparse();
	
	/* Create symbol table generator.*/
	cout << "##### Creating symbol table generator #####" << endl;
	SymbolTableGenerator* s = new SymbolTableGenerator(root);

	/* Generate symbol table. */
	cout << endl << "##### Generating symbol table #####" << endl;
	SymbolTable* sym = s->generateTable();

	/* Print the AST */
	cout << endl << "##### Printing AST via TreePrinter #####" << endl;
	cout << "Types showing as INVALID? Don't panic!" << endl 
		<< "Type resolution only occurs after check() has been "
		<< "called :)" << endl;
	treePrinter t(root);
	t.print(); 
	cout << endl << "##### Complete! #####" << endl;
	root->check();
	t.print();
	return 0;
}

int initTypeMap() { 
	typemap_add(TCHAR, "letter");
	typemap_add(TSTRING, "sentence");
	typemap_add(TNUMBER, "number");
	typemap_add(REFCHAR, "spider letter");
	typemap_add(REFSTRING, "spider sentence");
	typemap_add(REFNUMBER, "spider number");
	typemap_add(INVALIDTYPE, "INVALID");
	typemap_add(BOOLEAN, "Boolean");
	typemap_add(DASH, "-");
	typemap_add(LNOT, "~");
	typemap_add(LAND, "&&");
	typemap_add(LOR, "||");
	typemap_add(LEQU, "==");
	typemap_add(LNOTEQU, "!=");
	typemap_add(LGTHAN, ">");
	typemap_add(LGTHANEQ, ">=");
	typemap_add(LLTHAN, "<");
	typemap_add(LLTHANEQ, "<=");
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
