#include "NAssignment.hpp"
#include "TypeDefs.hpp"

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

int NAssignment::check() {
	int isValid = 1;
	string lvalID = lval->getID();

	Node* nodePtr = table->lookup(lvalID);

	/* Does the variable exist? If not, error. */
	if(nodePtr == NULL) {
		error_var_not_found(lvalID);
		isValid = 0;
	}
	
	else { 
		int lhsType = nodePtr->getType();
		int rhsType = rval->getType();

		/* Is the variable a keyword? */
		if(lhsType == KEYWORD) {
			error_keyword(lvalID);
			isValid = 0;
		}
		
		/*
		 * Does the type of the var match the type of the expression's
		 * RHS?
		 */
		else if(lhsType != rhsType) {
			error_type_mismatch(lvalID, lhsType, rhsType);
		}
		
	}

	/* Check the RHS of the assignment. */
	isValid &= rval->check();

	return isValid;
}
