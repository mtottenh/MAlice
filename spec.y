%token CHAR
%token IDENTIFIER
%token INTEGER
%token TERMINATOR

%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UNARY

%%

Function
	: String "()" "opened" Codeblock "closed"
	; 
Exp
	: Exp '+' Term
	| Exp '-' Term
	| Term
	;
Term
	: Term '*' Factor
	| Term '/' Factor
	| Factor
	;
Factor
	: '~' Value
	| Value
	;
Value
	: INTEGER
	| Identifier
	;
Type
	: "number"
	| "letter"
	;
Declare
	: "was a" Type
	;
Assign
	: "became" Exp
	| "became" CHAR
	;
Return
	: "said Alice"
	;
Code
	: Identifier Declare
	| Identifier Assign
	| Exp Return
	;
Codeblock
	: Code Seperator Codeblock
	| Code TERMINATOR
	; 
String
	: CHAR
	| String CHAR
	;
Seperator
	: TERMINATOR 
	| ','
	| "and"
	| "but"
	;
Identifier
	: IDENTIFIER
	;
