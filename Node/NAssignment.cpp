#include "NAssignment.hpp"
#include "TypeDefs.hpp"
#include "../Errors/TypeMap.hpp"

/* Constructors */

NAssignment::NAssignment() {
	name = "Assignment";
}

/* Special cases for VarDeclarationAssign. */
NAssignment::NAssignment(string id, Node* exp) {
	/* id = expression. */
	name = "Assignment";
	this->weight = -1;
	lval = new NIdentifier(id);
	rval = exp;
	children.push_back(lval);
	children.push_back(rval);
	nodeType = ASSIGNMENT;
}

NAssignment::NAssignment(string id, char *exp) {
	/* id = STRING */
	lval = new NIdentifier(id);
	rval = new NCharLit(exp);
	this->weight = 1;
	name = "Assignment";
	children.push_back(lval);
	children.push_back(rval);
	nodeType = ASSIGNMENT;

}
NAssignment::NAssignment(string id, string exp) {
	/* id = STRING */
	lval = new NIdentifier(id);
	name = "Assignment";
	this->weight = -1;
	rval = new NStringLit(exp);
	children.push_back(lval);
	children.push_back(rval);
	nodeType = ASSIGNMENT;

}

/* General case for identifier. */
NAssignment::NAssignment(Node* id, Node* exp) {
	/* id = EXPRESSION */
	lval = id;
	name = "Assignment";
	rval = exp;
	this->weight = -1;
	children.push_back(lval);
	children.push_back(rval);
	nodeType = ASSIGNMENT;
}

/* Method call. */
NAssignment::NAssignment(Node* call) {
	name = "Method assignment";
	children.push_back(call);
	this->weight = -1;
	nodeType = ASSIGNMENT;
}

/* Public methods. */

int NAssignment::check() {
	int isValid = 1;

	/*
	 * If children.size() == 1, we have a methdod call.
	 * Delegate checking to auxiliary checkMethod() procedure. 
	 */
	if (children.size() == 1) {
		return checkMethod();	
	}

	/* Otherwise, we have a standard assignment... */
	else {
		string lvalID = lval->getID();
	
		/* Check both sides of the assignment. */
		isValid &= rval->check();
		isValid &= lval->check();
	
		Node* nodePtr = table->lookup(lvalID);
	
		/* Does the variable exist? If not, error. */
		if(nodePtr == NULL) {
			printErrorHeader("assignment");	
			error_var_not_found(lvalID);
			isValid = 0;
		}
		
		else { 
			int lhsType = nodePtr->getType();
	
			/*
			 * If we have an array, we need the type of the array 
			 * access node as opposed to the array declaration node.
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
			 * Does the type of the var match the type of the 
			 * expression's RHS?
			 */
			if(!compareTypes(lhsType, rhsType)) {
				printErrorHeader("assignment");
				error_type_mismatch(lvalID, lhsType, rhsType);
			}
			
		}
	}

	return isValid;
}

/* Private methods. */

int NAssignment::checkMethod() {
	int isValid = 1;

	/* Check the child. */
	isValid &= children[0]->check();

	/* Lookup the method in the symbol table. */
	string methodName = children[0]->getID();
	Node* nodePtr = table->lookup(methodName);

	/* If we can't find the method, error. */
	if(nodePtr == NULL) {
		printErrorHeader("method call");
		error_var_not_found(methodName);
		isValid = 0;
	}
	
	/* Otherwise, check that we have a function (not a procedure) */
	else if(nodePtr->getNodeType() != PROCEDURE) {
		printErrorHeader("assignment");
		error_return_ignored(methodName);
		isValid = 0;
	}

	return isValid;
}
