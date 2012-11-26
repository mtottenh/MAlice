%{
#include <string>
#include <iostream>
#include <cstdarg>
#include "Node/NodeIncludes.hpp"
#include "TreePrinter/TreePrinter.hpp"
#include "Errors/TypeMap.hpp"
#include "TreeWalker/SymbolTableGenerator.hpp"
extern int yylex();
extern void yyerror (char *s, ...);
fileLocation generateLocation();
Node *root;

%}

%locations

/* Type Tokens */
%token<token> TCHAR TSTRING TREF TNUMBER REFCHAR REFSTRING REFNUMBER
/* Alice Keywords */
%token FUNC PROCEDURE OF WAS A LOOKING GLASS BECAME INC DEC CONTAINED 
HAD WHAT QUESTIONMARK EVENTUALLY ENOUGH TIMES THEN ELSE IF MAYBE TOO
ALICE FOUND THE ROOM VARDEC PARAMDEC WHICH UNSURE BECAUSE SAID SPOKE CODEBLOCK
DECLARATIONBLOCK STATLIST INPUTNODE

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
	struct value {
		char *string;
		int token;
	} values;
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
%type <node> VarDeclaration Return PredPrime Pred ParamListDec ParamList
%type <node> Loop
%type <func_dec> FunctionDec ProcedureDec
%type <node> Conditional Predicate Maybe
%type <node> BitExp Exp Term Factor Value ArrayVal Call 
%type <node> Assignment Read
%type <node> Statement PrintStatement
%type <node> StringLit Char
%type <id> Identifier Increment Decrement
%type <token> Type
%type <stat> StatementList
%type <dec_list> DeclarationList
%type <code_block> Codeblock
/* UNDCIDED ONES LOL */
 /*%type <stat> */
%%

/* We define a program as a list of Declarations, this allows global vaiables*/

program : DeclarationList { root = $1; root->isRoot(); }
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
	| error Declaration {yyerrok; }
	;
/* 
 * (TODO) Look into TREF
 */
Type
	: TNUMBER { $$ = TNUMBER; }
	| TCHAR { $$ = TCHAR; }
	| TSTRING {$$ = TSTRING;}
 /* We need to get the type 'number' rather than value etc. */
	| TREF TCHAR {$$ = REFCHAR;}
	| TREF TNUMBER {$$ = REFNUMBER;}
	| TREF TSTRING {$$ = REFSTRING;}
	;
VarDeclaration
	: Identifier WAS A Type {$$ = new NVariableDeclaration($1,$4);}
	| Identifier HAD Predicate Type {$$ = new NVariableDeclaration($1,$4,$3); }
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
	: Exp AND BitExp {$$ = new NBinOp($1, $3, AND);}
	| Exp OR BitExp {$$ = new NBinOp($1, $3, OR);}
	| Exp XOR BitExp {$$ = new NBinOp($1, $3, XOR);}
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
	: NOT Factor %prec UNARY {$$ = new NUnaryOp(NOT,$2);}
	| DASH Factor %prec UNARY {$$ = new NUnaryOp(DASH,$2);}
	| LNOT Factor %prec UNARY {$$ = new NUnaryOp(LNOT, $2);}
	| Value { $$ = $1;}
	;
Value
	: INTEGER {$$ = new NInteger($1); $$->setLocation(generateLocation());}
	| Identifier {$$ = $1;}
	| Call { $$ = $1;}
	| ArrayVal {$$ = $1;}
	| OBRACKET Predicate CBRACKET { $$ = $2;}
	| Char {$$ = $1;}
	| StringLit {$$ = $1;}
	;

Assignment
	: Identifier BECAME Predicate   { $$ = new NAssignment($1,$3);}
	| ArrayVal BECAME Predicate {$$ = new NAssignment($1,$3);}
	;
VarDeclarationAssignment
	: VarDeclaration OF Predicate 
	{ NVariableDeclaration* Declaration = (NVariableDeclaration *)$1; 
	  Node* Assignment =  new NAssignment(Declaration->getID(), $3);
	  $$ = new NStatementList(Declaration,Assignment);}
	;

Print
	: SAID ALICE {}
	| SPOKE {}
	;

PrintStatement
	: Predicate Print Separator { $$ = new NPrint($1);}
	;

Return
	: Found Predicate {$$ = new NReturn($2);}
	;

Found
	: ALICE FOUND {}
	;

Statement
	: Read {$$=$1;}
	| Conditional { $$ = $1;}
	| Loop {$$=$1;}
	| Call Separator {$$ = new NAssignment($1);}
	| NULLTOK {$$ = new NNullToken();}
	| Increment Separator {$$ = $1;}
	| Decrement Separator {$$ = $1;}
	| Codeblock {$$ = $1;}
	| Assignment Separator {$$ = $1;}
	| PrintStatement	{$$ = $1;}
	| Return Separator {$$ = $1;}
	| error Statement {yyerrok;}
	;

Call 
	: Identifier OBRACKET ParamList CBRACKET {$$ = new NMethodCall($1,$3);}
	| Identifier OBRACKET CBRACKET {$$ = new NMethodCall($1);}
	;
ParamList
	: ParamList COMMA Predicate { $1->children.push_back($3); }
	| Predicate { $$ = new NParamBlock($1); }
	;
Read
	: WHAT WAS Identifier QUESTIONMARK { $$ = new NInput($3); }
	| WHAT WAS ArrayVal QUESTIONMARK {$$ = new NInput($3); }
	;
Loop
	: EVENTUALLY OBRACKET Predicate CBRACKET BECAUSE StatementList 
		ENOUGH TIMES { $$ = new NLoop($3, $6); }
	;

Predicate
	: Pred {$$ = $1;}
	;

Pred
	: Pred LOR PredPrime {$$ = new NBinOp($1,$3,LOR);}
	| Pred LAND PredPrime {$$ = new NBinOp($1,$3,LAND);}
	| PredPrime {$$ = $1;}
	;

PredPrime
	: PredPrime LEQU BitExp  {$$ = new NBinOp($1,$3,LEQU);}
	| PredPrime LLTHAN BitExp {$$ = new NBinOp($1,$3,LLTHAN);}
	| PredPrime LLTHANEQ BitExp  {$$ = new NBinOp($1,$3,LLTHANEQ);}
	| PredPrime LGTHAN BitExp  {$$ = new NBinOp($1,$3,LGTHAN);}
	| PredPrime LGTHANEQ BitExp  {$$ = new NBinOp($1,$3,LGTHANEQ);}
	| PredPrime LNOTEQU BitExp {$$ = new NBinOp($1,$3,LNOTEQU);}
	| BitExp {$$ = $1;}
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
	: Identifier ARRINDO Predicate ARRINDC 
	{$$ = new NArrayAccess($1,$3); }
	;
 
Identifier
	: STRING {$$ = new NIdentifier($1);
				$$->setLocation(generateLocation());}
	;
StringLit
	: STRINGLIT {$$ = new NStringLit($1);
				$$->setLocation(generateLocation());}
Char
	: CHARLIT { $$ = new NCharLit($1); 
				$$->setLocation(generateLocation()); }
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
bool error_flag;
int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "ERROR: Usage is: " << argv[0] << " FILENAME "
			<< "[-d]" << endl;
		return 0;
	}
	error_flag = false;
	/* Open file from argv[1]. Quit if null. */
	FILE *input = fopen(argv[1],"r");
	if (input == NULL) {
 		cout << "ERROR: Could not open file " << argv[1] << endl;
 		return 0;
	}

	yyin = input;
	initTypeMap();
	int node = yyparse();

	if (root == NULL || node == 1 || error_flag) {
		cerr << "ERROR: Parse tree broke, stopping compiler" << endl;
		return -1;
	}	
	/* Create symbol table generator.*/
	cout << "##### Creating symbol table generator #####" << endl;
	SymbolTableGenerator* s = new SymbolTableGenerator(root);

	/* Generate symbol table. */
	cout << endl << "##### Generating symbol table #####" << endl;
	SymbolTable* sym = s->generateTable();

	/* Print the AST if debug flag enabled*/
	if(argc >= 3 && strcmp(argv[2], "-d") == 0) {
		cout << endl << "##### Printing AST via TreePrinter #####" << endl;
		cout << "Types showing as INVALID? Don't panic!" << endl 
			<< "Type resolution only occurs after check() has been "
			<< "called :)" << endl;
		treePrinter t(root);
		t.print();
	}

	cout << endl << "##### Semantic Analysis (check()) #####" << endl;
	root->check();
//	t.print();
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
	typemap_add(VOID, "void");
	typemap_add(DASH, "-");
	typemap_add(LNOT, "!");
	typemap_add(LAND, "&&");
	typemap_add(LOR, "||");
	typemap_add(LEQU, "==");
	typemap_add(LNOTEQU, "!=");
	typemap_add(LGTHAN, ">");
	typemap_add(LGTHANEQ, ">=");
	typemap_add(LLTHAN, "<");
	typemap_add(LLTHANEQ, "<=");
	typemap_add(PLUS, "+");
	typemap_add(MULT, "*");
	typemap_add(DIV, "/");
	typemap_add(MOD, "%");
	typemap_add(XOR, "^");
	typemap_add(AND, "&");
	typemap_add(OR, "|");
	typemap_add(NOT, "~");
	typemap_add(VOID, "void");
	typemap_add(INC, "ate");
	typemap_add(DEC, "drank");
	typemap_add(SAID, "said Alice/spoke");
	return 1;
}

void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);
  error_flag = true;
  if(yylloc.first_line)
   vfprintf(stderr, s, ap);
  if (yylval.values.string != NULL)
  fprintf(stderr, " on Line(s) %d-%d. Column %d-%d: Token: %s", yylloc.first_line,
	yylloc.last_line, yylloc.first_column, yylloc.last_column, yylval.values.string);
  else
  fprintf(stderr, " on Line(s) %d-%d. Column %d-%d: Token: %d", yylloc.first_line,
	yylloc.last_line, yylloc.first_column, yylloc.last_column, yylval.values.token);
  fprintf(stderr, "\n");
}

fileLocation generateLocation()
{
	fileLocation data;
	data.startLine = yylloc.first_line;
	data.endLine = yylloc.last_line;
	data.startColumn = yylloc.first_column;
	data.endColumn = yylloc.last_column;
	return data;
}
