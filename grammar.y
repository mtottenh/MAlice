%{
#include <string>
#include <iostream>
#include <cstdarg>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Node/NodeIncludes.hpp"
#include "TreeUtils/TreePrinter.hpp"
#include "Errors/TypeMap.hpp"
#include "TreeUtils/SymbolTableGenerator.hpp"
#include "CodeGeneration/ASTVisitor.hpp"
#include "CodeGeneration/x86Visitor.hpp"
#include "CodeGeneration/RefCountGenerator.hpp"
#include "CodeGeneration/TreeOptimiser.hpp"
    
extern int yylex();
extern void yylex_destroy();
extern void yyerror (char *s, ...);
FileLocation generateLocation();
Node *root;

%}

%locations

/* Type Tokens */
%token<token> TCHAR TSTRING TREF TNUMBER REFCHAR REFSTRING REFNUMBER
/* Alice Keywords */
%token FUNC PROCEDURE OF WAS A LOOKING GLASS BECAME INC DEC CONTAINED 
HAD WHAT QUESTIONMARK EVENTUALLY ENOUGH TIMES THEN ELSE IF MAYBE TOO
ALICE FOUND THE ROOM VARDEC PARAMDEC WHICH UNSURE BECAUSE SAID SPOKE
NULLTOK COMMA QUOTE

/*Node Types used in semantic analysis*/
%token ARRAYACCESS ASSIGNMENT BINOP IDENTIFIER CALL CODEBLOCK
DECLARATIONBLOCK STATLIST INPUTNODE RETURN CONDITIONAL LOOP GENERIC_NODE 
PARAMBLOCK PRED UNARYOP SEPARATOR INVALIDTYPE BOOLEAN VOID ENDIF

/* Primitives */
%token <string> CHARLIT STRING STRINGLIT
%token <token> INTEGER
/* Unary Operators */
%token <token> DASH PLUS MULT DIV MOD 
/* Bitwise operators*/
%token <token> XOR AND OR NOT
/* Logical operators */
%token <token> LAND LOR LEQU LNOT LNOTEQU LGTHAN LGTHANEQ LLTHAN LLTHANEQ
/* bracket and braces */
%token <token> OBRACKET CBRACKET ARRINDO ARRINDC
%token <token> OBRACE CBRACE 
/*Built in functions */
%token <token> PRINT

/* Start token */
%start Program 

/* Precedence of UNARY operators */
%left UNARY

/* Possible values stored to yylval */
%union {
	char *string;
	int token;
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

/* A mapping of types to Non-Terminals */
%type <node> Declaration Program ParameterDec VarDeclarationAssignment
%type <node> VarDeclaration Return ParamListDec ParamList
%type <node> Loop Conditional Predicate Maybe PredPrime
%type <func_dec> FunctionDec ProcedureDec
%type <node> BitExp Exp Term Factor Value ArrayVal StringLit Char  
%type <dec_list> DeclarationList
%type <stat> StatementList
%type <code_block> Codeblock
%type <node> Statement PrintStatement Call Assignment Read
%type <id> Identifier Increment Decrement
%type <token> Type

%%

/* We define a program as a list of Declarations, this allows global vaiables*/

/* Reduce to a Program and at root */
Program : DeclarationList { root = $1; root->setRoot(); }
	;

/* Create an AST node whose children are declarations. */
DeclarationList
	: DeclarationList Declaration 
	{ $1->addChild($2); }
	| Declaration 
	{ $$ = new NDeclarationBlock(); $$->addChild($1); } 
	;
/*
 * The value of a declaration node is just defined as the 
 * evaluation of one of the possible things it declares
 * i.e. a generic Declaration node should not appear int the tree
 */
Declaration
	: VarDeclaration Separator { $$=$1 ; }
	| VarDeclaration TOO Separator { $$ = $1; }
	| VarDeclarationAssignment Separator { $$ = $1; }
	| FunctionDec	 { $$ = $1; }
	| ProcedureDec     { $$= $1; }
	| error Separator Declaration {yyerrok; $$ = $3; }
	;
/* 
 * The types of primitives, as defined at the top of the file.
 */
Type
	: TNUMBER { $$ = TNUMBER; }
	| TCHAR { $$ = TCHAR; }
	| TSTRING { $$ = TSTRING; }
	| TREF TCHAR { $$ = REFCHAR; }
	| TREF TNUMBER { $$ = REFNUMBER; }
	| TREF TSTRING { $$ = REFSTRING; }
	;

/*
 * A declaration of a variable is reduced into an AST node which is
 * given an identifier and its corresponding type.
 */
VarDeclaration
	: Identifier WAS A Type { $$ = new NVariableDeclaration($1,$4); }
	| Identifier HAD Predicate Type { $$ = new NVariableDeclaration($1,$4,$3); }
	;

/* 
 * Assigning a Predicate to an identifier creates a NAssignment node with them
 * as children.
 */
Assignment
	: Identifier BECAME Predicate   { $$ = new NAssignment($1,$3); }
	| ArrayVal BECAME Predicate { $$ = new NAssignment($1,$3); }
	;

/* A variable declaration with an immediate assignment creates a
 * NStatementList with a declaration as one child and an assignment as
 * another. It gets the declaration identifier and adds that to the
 * NAssignmentNode. It is also neccessary to set the location in code of the
 * identifier as it will be fresh (and doesn't explicity exist in code).
 */
VarDeclarationAssignment
	: VarDeclaration OF Predicate 
	{ NVariableDeclaration* declaration = (NVariableDeclaration *)$1;
	  NIdentifier* identifier = new NIdentifier(declaration->getID());
	  identifier->setLocation(declaration->getLocation()); 
	  Node* assignment =  new NAssignment(identifier, $3);
	  $$ = new NStatementList(declaration,assignment); }
	;

/*
 * A function with optional parameters creates a NFunctionDeclaration node,
 * with its identifier, optional parameters, type and code as children.
 */
FunctionDec
	: Func Identifier OBRACKET ParamListDec CBRACKET CONTAINED A Type Codeblock
	{ $$ = new NFunctionDeclaration($2,$4,$9,$8); }
	| Func Identifier OBRACKET CBRACKET CONTAINED A Type Codeblock 
	{ $$ = new NFunctionDeclaration($2,$8,$7); }
	;

/* A procedure with option parameters creates a NFunctionDeclaration node,
 * with its identifier, optional parameters and code as children.
 */
ProcedureDec
	:  Procedure Identifier OBRACKET ParamListDec CBRACKET Codeblock
	{ $$ = new NFunctionDeclaration($2,$4,$6); }
	|  Procedure Identifier OBRACKET CBRACKET Codeblock 
	{ $$ = new NFunctionDeclaration($2,$5);}
	;

/*
 * A list of parameters is created as children of an NParamDeclarationBlock
 * node.
 */
ParamListDec
	: ParameterDec { $$ = new NParamDeclarationBlock($1); }
	| ParamListDec COMMA ParameterDec { $1->addChild($3); }
	;

/*
 * A parameter declaration is defined as a type followed by an identifier
 * relating to a variable of that type. We create a NVariableDeclaration node
 * with these as children.
 */ 

ParameterDec
	: Type Identifier { $$ = new NVariableDeclaration($2,$1); }
	;

/*
 * The start of a chain of expressions; logical, bitwise, unary and binary.
 * Their precedence goes from low to high.
 */
Predicate
	: Predicate LOR PredPrime { $$ = new NBinOp($1,$3,LOR); }
	| Predicate LAND PredPrime { $$ = new NBinOp($1,$3,LAND); }
	| PredPrime { $$ = $1; }
	;

PredPrime
	: PredPrime LEQU BitExp  { $$ = new NBinOp($1,$3,LEQU); }
	| PredPrime LLTHAN BitExp { $$ = new NBinOp($1,$3,LLTHAN); }
	| PredPrime LLTHANEQ BitExp  { $$ = new NBinOp($1,$3,LLTHANEQ); }
	| PredPrime LGTHAN BitExp  { $$ = new NBinOp($1,$3,LGTHAN); }
	| PredPrime LGTHANEQ BitExp  { $$ = new NBinOp($1,$3,LGTHANEQ); }
	| PredPrime LNOTEQU BitExp { $$ = new NBinOp($1,$3,LNOTEQU); }
	| BitExp {$$ = $1;}
	;
BitExp
	: Exp AND BitExp { $$ = new NBinOp($1, $3, AND); }
	| Exp OR BitExp { $$ = new NBinOp($1, $3, OR); }
	| Exp XOR BitExp { $$ = new NBinOp($1, $3, XOR); }
	| Exp { $$ = $1; }
	;
Exp
	: Exp PLUS Term { $$ = new NBinOp($1, $3, PLUS); }
	| Exp DASH Term { $$ = new NBinOp($1, $3, DASH); }
	| Term { $$ = $1; }
	;
Term
	: Term MULT Factor { $$ = new NBinOp($1, $3, MULT); }
	| Term DIV Factor { $$ = new NBinOp($1, $3, DIV); }
	| Term MOD Factor { $$ = new NBinOp($1, $3, MOD); }
	| Factor { $$ = $1; }
	;
/*
 * Unary Operators are created with the NUnaryOp class which accepts
 * the operator token returned from the lexer
 * a pointer to the subexpression to apply to operator too
 */
Factor
	: NOT Factor %prec UNARY { $$ = new NUnaryOp(NOT,$2); }
	| DASH Factor %prec UNARY { $$ = new NUnaryOp(DASH,$2); }
	| PLUS Factor %prec UNARY { $$ = new NUnaryOp(PLUS, $2); }
	| LNOT Factor %prec UNARY { $$ = new NUnaryOp(LNOT, $2); }
	| Value { $$ = $1; }
	;
Value
	: INTEGER { $$ = new NInteger(yylval.values.token); $$->setLocation(generateLocation()); }
	| Identifier { $$ = $1; }
	| Call { $$ = $1; }
	| ArrayVal { $$ = $1; }
	| OBRACKET Predicate CBRACKET { $$ = $2; }
	| Char { $$ = $1; }
	| StringLit { $$ = $1; }
	;

Print
	: SAID ALICE {}
	| SPOKE {}
	;

/*
 * A generalised print statement for all expressions. Note: this simplifies
 * down to simpler entities like Identifiers or Integers.
 */
PrintStatement
	: Predicate Print Separator { $$ = new NPrint($1); }
	;
/*
 * A general return statement which creates an NReturn node with the Predicate
 * as its child.
 */
Return
	: Found Predicate { $$ = new NReturn($2); }
	;

Found
	: ALICE FOUND {}
	;

/*
 * A list of statements which can exist within a codeblock. Statments are set
 * as the children of an NStatement node.
 */
StatementList
	: StatementList Statement { $1->addChild($2); }
	| Statement{ $$ = new NStatementList(); $$->addChild($1); }
	;

Statement
	: Read { $$=$1; }
	| Conditional { $$ = $1; }
	| Loop { $$=$1; }
	| Call Separator { $$ = new NAssignment($1); }
	| NULLTOK { $$ = new NNullToken(); $$->setLocation(generateLocation()); }
	| Increment Separator { $$ = $1; }
	| Decrement Separator { $$ = $1; }
	| Codeblock { $$ = $1; }
	| Assignment Separator { $$ = $1; }
	| PrintStatement	{ $$ = $1; }
	| Return Separator { $$ = $1; }
	| error Separator Statement { $$ = $3; yyerrok; }
	;

/*
 * A method call, either function or procedure, creates an NMethodCall node
 * with an identifier and optional parameters.
 */
Call 
	: Identifier OBRACKET ParamList CBRACKET { $$ = new NMethodCall($1,$3); }
	| Identifier OBRACKET CBRACKET { $$ = new NMethodCall($1); }
	;

/*
 * A list of parameters, all of type predicate. These are added as children of
 * a NParamBlock node.
 */
ParamList
	: ParamList COMMA Predicate { $1->addChild($3); }
	| Predicate { $$ = new NParamBlock($1); }
	;

/*
 * A block of code, denoted by an opened statement followed by optional code,
 * followed by a closed statement. Note: Declarations without executable code
 * is not allowed within a CodeBlock under the MAlice language, but executable
 * code without declarations is valid syntax. An empty code block is also
 * allowed.
 */
Codeblock
	: OBRACE DeclarationList StatementList CBRACE 
	{ $$ = new NCodeBlock($2, $3); }
	| OBRACE StatementList CBRACE { $$ = new NCodeBlock($2); }
	| OBRACE CBRACE { $$ = new NCodeBlock();
	 $$->setLocation(generateLocation()); }
	; 

/* A general read statement. */
Read
	: WHAT WAS Identifier QUESTIONMARK { $$ = new NInput($3); }
	| WHAT WAS ArrayVal QUESTIONMARK { $$ = new NInput($3); }
	;

/*
 * A loop statement with a predicate and a statement list as children of a
 * NLoop node.
 */
Loop
	: EVENTUALLY OBRACKET Predicate CBRACKET BECAUSE StatementList 
		ENOUGH TIMES { $$ = new NLoop($3, $6); }
	;
/*
 * A conditional if statement with a Predicate and a statement list. Else if
 * cases are handled by the maybe child.
 */
Conditional
	: IF OBRACKET Predicate CBRACKET THEN StatementList Maybe
	 { $$ = new NConditional($3,$6,$7); }
	;

/*
 * The maybe non-terminal takes into account three cases.
 * 1) The statement has an else, the ends.
 * 2) The statement does not have an else, so it ends.
 * 3) The statement has an else if followed by a statement list then another
 * maybe.
 */
Maybe
	: ELSE StatementList EndIf { $$ = $2; }
	| EndIf { $$ = new NEndIf(); $$->setLocation(generateLocation()); }
	| ELSE MAYBE OBRACKET Predicate CBRACKET THEN StatementList Maybe 
	{ $$ = new NConditional($4,$7,$8); }
	;

/*
 * An increment expression creates a NInc node with the identifier or array
 * value as its child.
 */
Increment
	: Identifier INC { $$ = new NInc($1); }
	| ArrayVal INC { $$ = new NInc($1); }
	;

/*
 * A decrement expression creates a NDec node with the identifier or array
 * value as its child.
 */
Decrement
	: Identifier DEC { $$ = new NDec($1); }
	| ArrayVal DEC { $$ = new NDec($1); }
	;	
/* Three potential Separators */
Separator
	: NULLTOK {}
	| SEPARATOR {}
	| COMMA {}
	;

/* Access to an array specifying the identifier and its position as a
 * predicate.
 */
ArrayVal
	: Identifier ARRINDO Predicate ARRINDC 
	{ $$ = new NArrayAccess($1,$3); }
	;

/* An identifier creates a NIdentifier node with a string as its only child.
 * We set the location in code of an NIdentifier for use in the semantic
 * checking.
 */
Identifier
	: STRING { $$ = new NIdentifier($1);
		 $$->setLocation(generateLocation()); }
	;

/*
 * A string literal which creates a NStringLit node with a string as its only
 * child and a set location in code for use in semantic analysis.
 */
StringLit
	: STRINGLIT { $$ = new NStringLit($1);
		     $$->setLocation(generateLocation()); }
/*
 * A char literal which creates a NCharLit node with a character as its only
 * child and a set location in code for use in semantic analysis.
 */
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
		cerr << "ERROR: Usage is: " << argv[0] << " FILENAME "
			<< "[-d]" << endl;
		return EXIT_FAILURE;
	}
	error_flag = false;
	/* Open file from argv[1]. Quit if null. */
	FILE *input = fopen(argv[1],"r");
	if (input == NULL) {
 		cerr << "ERROR: Could not open file " << argv[1] << endl;
 		return EXIT_FAILURE;
	}

	//Parse the input!
	yyin = input;
	initTypeMap();
	int node = yyparse();

	if (root == NULL || node == 1 || error_flag) {
		cerr << "ERROR: Parse tree broke, stopping compiler" << endl;
		return EXIT_FAILURE;
	}	

	SymbolTableGenerator s(root);
	/* Generate symbol table. */
	int isValid = s.generateTable();

	// Print the AST if debug flag enabled
	if(argc >= 3 && strcmp(argv[2], "-d") == 0) {
		cout << endl << "##### Printing AST via TreePrinter #####" << endl;
		cout << "Types showing as INVALID? Don't panic!" << endl 
			<< "Type resolution only occurs after check() has been "
			<< "called :)" << endl;
		treePrinter t(root);
		t.print();
	}

	//Check that the AST is semantically valid.
	isValid &= root->check();
    
    ASTVisitor *rc = new RefCountGenerator();
    ASTVisitor *t = new TreeOptimiser();
    ASTVisitor *v = new x86Visitor();
    /* optimise generated code by pruning tree */
    root->accept(rc);
    root->accept(t);
    /* generate code using x86Visitor*/
//    v->init(root);
    root->accept(v);
	// TODO
	// There may be a cleaner way to encapsulate this
    /* check if is root in VarDecList node if so
     * call  init, then do normal visit 
     * then call genfuncDefs
     */
//	v->generateFunctionDefinitions();

    /*create the output file*/
    string outputFname(argv[1]);
    size_t pos = outputFname.find(".alice");
    outputFname = outputFname.substr(0,pos);
    outputFname = outputFname + ".asm";
    FILE *output = fopen(outputFname.c_str(),"w");
    if (output != NULL) {
        fputs(v->getAssembly().c_str(),output);
       // fputs("\n\n\tpop rbp\n\tsys.exit\n",output);
        fclose(output);
        /* assemble with nasm */
        /* TODO - Ask mark whether it would be easier
         * to just create a shell script, ie not relying
         * on nasm/ld to be under /usr/bin
         */
        cerr << "Assembling with nasm, output filename: " << outputFname << endl;
        if (fork() == 0) {
            execl("/usr/bin/nasm","/usr/bin/nasm", "-f elf64",  "-g -F stabs",
                    outputFname.c_str(),(char *) 0);
        } else {
            int status;
            wait(&status);
            /* link with ld */
            pos = outputFname.find(".asm");
            string objFname = outputFname.substr(0,pos) + ".o";
            outputFname = "-o" + outputFname.substr(0,pos);
		    cerr << "\nOutput filename:  " << outputFname << "\t" << objFname <<endl;
            execl("/usr/bin/gcc","/usr/bin/gcc","-g",objFname.c_str(),
                        outputFname.c_str(),(char*)0);
        }
    } else {
        cerr << "error opening output file for writing: " << outputFname << endl;
    }
	//Finish up with a bit of memory management.
	delete root;
    delete v;
	fclose(input);
    fclose(output);
	yylex_destroy();
	return isValid ? EXIT_SUCCESS : EXIT_FAILURE;
}

//A map of enumerated values to their relevant types.
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

FileLocation generateLocation()
{
	FileLocation data; 
	data.startLine = yylloc.first_line;
	data.endLine = yylloc.last_line;
	data.startColumn = yylloc.first_column;
	data.endColumn = yylloc.last_column;
	return data;
}
