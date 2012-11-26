#ifndef ___NMETHODCALL__
#define ___NMETHODCALL__

#include "NMethodCall.hpp"
#include "TypeDefs.hpp"

NMethodCall::NMethodCall() 
{ 
	name = "function/procedure call"; 
}

NMethodCall::NMethodCall(NIdentifier* id, Node* params) {
	name = id->getID();
	children.push_back(params);
	delete(id);
}

NMethodCall::NMethodCall(NIdentifier* id) {
	name = id->getID();
	type = VOID;
	delete(id);
}

int NMethodCall::resolveType() {
	Node* nodePtr = table->lookup(name);
	if (nodePtr == NULL)
		return INVALIDTYPE;
	return nodePtr->getType();
}
int NMethodCall::getType() {
	return resolveType();
}

/* TODO refactor this check function
 */
int NMethodCall::check() {
	int isValid = 1;

	Node* nodePtr = table->lookup(name);
	
	/* Does the method exist in scope? */
	if(nodePtr == NULL) {
		printErrorHeader("method call");
		error_var_not_found(name);
		isValid = 0;
	}
	
	else { 
		/* Is the identifier a function or procedure? */
		if(!(nodePtr->getNodeType() == FUNC 
				|| nodePtr->getNodeType() == PROCEDURE)) {
			printErrorHeader("method call");
			error_not_func(name);
			isValid = 0;
		}
		/* Check that the parameters (child nodes) are valid. */
		isValid &= Node::check();	
		/*
		 * Get the types of the expected paramaters. We have a list of
		 * parameters if the size of the function node's children = 2.
		 */
		if(nodePtr->children.size() == 2) {
			Node* param = nodePtr->children[1];
			isValid &= checkParams(param);	
		}
			
	}



	return isValid;
}

int NMethodCall::checkParams(Node* funcParams) {
	int isValid = 1;

	node_children_t thisParams = children[0]->getChildren();
	int thisSize = thisParams.size();
	int expectedSize = funcParams->children.size();
	
	/* Do we have the same number of expected and given arguments? */
	if(thisSize != expectedSize) {
		printErrorHeader("method call");
		error_num_args(name, expectedSize, thisSize);
		isValid = 0;
	}

	/* Compare the arguments and ensure they are of the same type. */
	node_children_t::iterator funcIt = funcParams->children.begin();
	node_children_t::iterator givenIt = thisParams.begin();

	int expectedType, givenType;
	string varName;
	while(funcIt != funcParams->children.end() 
			&& givenIt != thisParams.end()) {
		expectedType = (*funcIt)->getType();
		givenType = (*givenIt)->getType();
		if(!compareTypes(expectedType, givenType)) {
			varName = (*givenIt)->getID();
			printErrorHeader("method call");
			error_type_mismatch(varName, givenType, expectedType);
			isValid = 0;
		}

		++funcIt; ++givenIt;
	}

	return isValid;
}

#endif
