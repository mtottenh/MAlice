#include "NAssignment.hpp"
#include "TypeDefs.hpp"
#include "../Errors/TypeMap.hpp"

NAssignment::NAssignment() {
	name = "Assignment";
}

/* special cases for VarDeclarationAssign */
NAssignment::NAssignment(string id, Node* exp) {
	name = "Assignment";
	lval = new NIdentifier(id);
	rval = exp;
	children.push_back(lval);
	children.push_back(rval);
}
NAssignment::NAssignment(string id, char *exp) {
	lval = new NIdentifier(id);
	rval = new NCharLit(exp);
	name = "Assignment";
	children.push_back(lval);
	children.push_back(rval);

}
NAssignment::NAssignment(string id, string exp) {
	lval = new NIdentifier(id);
	name = "Assignment";
	rval = new NString(exp);
	children.push_back(lval);
	children.push_back(rval);

}
/* General cases for Identifier */
NAssignment::NAssignment(Node* id, Node* exp) {
	lval = id;
	name = "Assignment";
	rval = exp;
	children.push_back(lval);
	children.push_back(rval);

}
NAssignment::NAssignment(Node* id, char *exp) {
	lval = id;
	name = "Assignment";
	rval = new NCharLit(exp);
	children.push_back(lval);
	children.push_back(rval);

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
		cout << "RHS TYPE : " << typemap_get(rhsType) << endl;
		/*
		 * Does the type of the var match the type of the expression's
		 * RHS?
		 */
		if(lhsType != rhsType) {
			error_type_mismatch(lvalID, rhsType, lhsType);
		}
		
	}

	/* Check the RHS of the assignment. */
	isValid &= rval->check();

	return isValid;
}
