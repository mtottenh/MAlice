%{
#include "y.tab.h"
%}

char		[a-zA-Z]
string 		{char}[a-zA-Z0-9]*
integer		[0-9]+
terminator	[.(too.)]
seperator	[,(and)(but)(then){terminator}]

%%

{char}			{ return CHAR; }
{string}		{ return STRING; }
{integer}		{ return INTEGER; }
{terminator} 	{ return TERMINATOR; }
{seperator}		{ return SEPERATOR; }
