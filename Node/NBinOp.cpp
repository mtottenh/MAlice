#include "NBinOp.hpp"
#include "TypeDefs.hpp"

NBinOp::NBinOp(Node* left, Node* right, BinaryOperator op) : binOp(op) {
	name = "Binary Operator";
	children.push_back(right);
	children.push_back(left);
}

int NBinOp::check() {
	int isValid = 1;
	
	/* Are the operands valid? And are they numbers? */
	node_children_t::iterator it;
	Node* nodePtr;
	for(it = children.begin(); it != children.end(); ++it) {
		nodePtr = *it;

		isValid &= nodePtr->check();
		
		int nodeType = nodePtr->getType();
		if (nodeType != TNUMBER) {
			error_type_mismatch(nodePtr->getID(), nodeType, 
						TNUMBER);
			isValid = 0;
		}
	}
		
	return isValid;
}
