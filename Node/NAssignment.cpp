#include "NAssignment.hpp"

NAssignment::NAssignment() {
	name = "Assignment";
}

/* special cases for VarDeclarationAssign */
NAssignment::NAssignment(string id, Node* exp) {
	lval = new NIdentifier(id);
	rval = exp;
}
NAssignment::NAssignment(string id, char *exp) {
	lval = new NIdentifier(id);
	rval = new NChar(exp);
}
NAssignment::NAssignment(string id, string exp) {
	lval = new NIdentifier(id);
	rval = new NString(exp);
}
/* General cases for Identifier */
NAssignment::NAssignment(Node* id, Node* exp) {
	lval = id;
	rval = exp;
}
NAssignment::NAssignment(Node* id, char *exp) {
	lval = id;
	rval = new NChar(exp);
}
/* Add cases for assigning things to strings */

