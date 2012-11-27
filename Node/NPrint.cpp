#include "NPrint.hpp"
#include "TypeDefs.hpp"

NPrint::NPrint() {
	name = "Print";
	nodeType = PRINT;
}
/* TODO consider pushing this into an NStringLit child node?
 * it would simplify the check function
 */
NPrint::NPrint(const char* msg) {
	message = strdup(msg); 
	name = "Print";
	nodeType = PRINT;
}

NPrint::NPrint(Node* child) { 
	name = "print";
	children.push_back(child); 
	nodeType = PRINT;
}

int NPrint::check() {
	int isValid = 1;

	/* If we have no child nodes, we have the const char* case. */
	if(children.size() == 0) {
		return isValid;
	}

	/* If we have a child node, we need to check it via superclass call */
	isValid &= Node::check();

	/* Otherwise, check that the node has a type that can be printed. */
	int childType = children[0]->getType(); 

	if(childType != TNUMBER && childType != TCHAR && childType != TSTRING) {
		printErrorHeader("print");
		error_type_mismatch(SAID, childType, "number/letter/sentence");
		isValid = 0;
	}

	return isValid;
}
