#include "NAssignment.hpp"

NAssignment::NAssignment() {
	name = "Assignment";
}

/* special cases for VarDeclarationAssign */
NAssignment::NAssignment(string id, Node* exp) {
	name = "Assignment";
	lval = new NIdentifier(id);
	rval = exp;
}
NAssignment::NAssignment(string id, char *exp) {
	lval = new NIdentifier(id);
	rval = new NCharLit(exp);
	name = "Assignment";
}
NAssignment::NAssignment(string id, string exp) {
	lval = new NIdentifier(id);
	name = "Assignment";
	rval = new NString(exp);
}
/* General cases for Identifier */
NAssignment::NAssignment(Node* id, Node* exp) {
	lval = id;
	name = "Assignment";
	rval = exp;
}
NAssignment::NAssignment(Node* id, char *exp) {
	lval = id;
	name = "Assignment";
	rval = new NCharLit(exp);
}
/* Add cases for assigning things to strings */

