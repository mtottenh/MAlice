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
	rval = new NStringLit(exp);
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
/* TODO see if this case needs removing */
NAssignment::NAssignment(Node* id, char *exp) {
	lval = id;
	name = "Assignment";
	cerr << "debuging chars vs strings: ";
	rval = new NCharLit(exp);
	children.push_back(lval);
	children.push_back(rval);

}
/* Add cases for assigning things to strings */
NAssignment::NAssignment(Node* call)
{
	name = "Method assignment";
	children.push_back(call);
}
/* TODO Refactor check() *///
int NAssignment::check() {
	int isValid = 1;

	if (children.size() == 1)
	{
		isValid &= children[0]->check();

		//TODO Null check
		Node* nodePtr = table->lookup(children[0]->getID());

		//A single method call without an assignment
		if(nodePtr->getNodeType() != PROCEDURE) {
			//TODO Implement proper error function here
			cerr << "Function return value ignored: " <<
				nodePtr->getID() << endl;
			isValid = 0;
		}
	}
	else
	{
		string lvalID = lval->getID();
	
		/* Check both sides of the assignment. */
		isValid &= rval->check();
		isValid &= lval->check();
	
		Node* nodePtr = table->lookup(lvalID);
	
		/* Does the variable exist? If not, error. */
		if(nodePtr == NULL) {
			error_var_not_found(lvalID);
			isValid = 0;
		}
		
		else { 
			int lhsType = nodePtr->getType();
	
			/*
			 * If we have an array, we need the type of the array access
			 * node as opposed to the array declaration node.
			 */
			if(lhsType == REFNUMBER || lhsType == REFCHAR
					|| lhsType == REFSTRING) {
				lhsType = children[0]->getType();
			}
			
			int rhsType = rval->getType();
			if (rval->getNodeType() == INPUTNODE) {
				return isValid;
			}
			/*
			 * Does the type of the var match the type of the expression's
			 * RHS?
			 */
			if(!compareTypes(lhsType, rhsType)) {
				error_type_mismatch(lvalID, lhsType, rhsType);
			}
			
	}
	}
	return isValid;
}
