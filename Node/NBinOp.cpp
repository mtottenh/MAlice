#include "NBinOp.hpp"
#include "TypeDefs.hpp"

NBinOp::NBinOp(Node* left, Node* right, int op) {
	this->op = op;
	this->name = "Binary Operator";
	children.push_back(left);
	children.push_back(right);
}
int NBinOp::getType() const {
	return resolveType();
}
int NBinOp::resolveType() const{
	/* Do the types of the children match? */
	int t1 = children[0]->getType();
	int t2 = children[1]->getType();

	if(t1 != t2 && t1 != TNUMBER && t2 != TCHAR) {
		return INVALIDTYPE;
	}

	else {
		return t1;
	}
}

int NBinOp::check() {
	int isValid = 1;

	this->type = resolveType();
	
	/* Are the operands valid? Superclass check will evaluate child nodes */
	isValid &= Node::check();
	
	/*
	 * Is the result of resolveType invalid? If so, we have a type
	 * mismatch.
	 */
	if(type == INVALIDTYPE) {
		int t1 = children[0]->getType();
		int t2 = children[1]->getType();
		
		/* If t1 is a num or char, we expect t2 to be of type t1. */
		if(t1 == TNUMBER || t1 == TCHAR) {
			error_type_mismatch(op, t2, t1);
			isValid = 0;
		}
		
		/* If t2 is a num or char, we expect t1 to be of type t2. */
		else if(t2 == TNUMBER || t2 == TCHAR) {
			error_type_mismatch(op, t1, t2);
			isValid = 0;
		}

		/* Otherwise both are invalid. */
		else {
			error_type_mismatch(op, t1, "number/letter");
			error_type_mismatch(op, t2, "number/letter");
			isValid = 0;
		}
	}
		
	return isValid;
}
