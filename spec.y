%token CHAR
%token STRING
%token INTEGER
%token SEPERATOR
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
LogExp
	: LogExp '&' Exp 
	| LogExp '|' Exp 
	| LogExp '^' Exp
	| Exp
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
	: '~' Value %prec UNARY
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
	: "became" LogExp
	| "became" CHAR
	;
Return
	: "said Alice"
	;
Code
	: Identifier Declare
	| Identifier Assign
	| LogExp Return
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
	: STRING
	;
