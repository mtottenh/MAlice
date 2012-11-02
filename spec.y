%token CHAR
%token STRING
%token INTEGER
%token TERMINATOR
%token SEPARATOR

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
	| STRING
	;
Type
	: "number"
	| "letter"
	;
Declare
	: STRING "was a" Type
	;
Assign
	: STRING "became" Exp
	| STRING "became" CHAR
	;
Return
	: LogExp "said Alice"
	;
BuiltInFunc
	: STRING "drank"
	| STRING "ate"
	;
Code
	: Declare
	| Assign
	| Return
	| BuiltInFunc
	|
	;
Codeblock
	: Code SEPARATOR Codeblock
	| Code TERMINATOR
	;
