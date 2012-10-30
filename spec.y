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
	: STRING "()" "opened" Codeblock "closed"
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
	| IDENTIFIER
	;
Type
	: "number"
	| "letter"
	;
Declare
	: IDENTIFIER "was a" Type
	;
Assign
	: IDENTIFIER "became" Exp
	| IDENTIFIER "became" CHAR
	;
Return
	: Exp "said Alice"
	;
Code
	: Declare
	| Assign
	| Return
	;
Codeblock
	: Code SEPARATOR Codeblock
	| Code TERMINATOR
	;
