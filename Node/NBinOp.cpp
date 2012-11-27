#include "NBinOp.hpp"
#include "TypeDefs.hpp"

NBinOp::NBinOp(Node* left, Node* right, int op) {
	this->op = op;
	this->name = "Binary Operator";
	children.push_back(left);
	children.push_back(right);
	nodeType = BINOP;
}

int NBinOp::getType() {
	return resolveType();
}

int NBinOp::resolveType()  {
	/* Is the operator a boolean binary operator? */
	if(isBoolean()) {
		return BOOLEAN;
	} 
	
	/* Do the types of the children match? If not, invalid. */
	int t1 = children[0]->getType();
	int t2 = children[1]->getType();

	if(!Node::compareTypes(t1, t2)) {
		return INVALIDTYPE;
	}

	/* Are they numbers or letters? If not, invalid. */
	else if(t1 != TNUMBER && t1 != TCHAR) {
		return INVALIDTYPE;
	}

	/* Yay, everything is fine :D */
	else {
		return t1;
	}
}

int NBinOp::check() {
	int isValid = 1;

	this->type = resolveType();

	/* If this of type boolean, then we need to perform the boolean check */
	if(type == BOOLEAN) {
		return checkBoolean();
	}

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
			printErrorHeader("binary operator");
			error_type_mismatch(op, t2, t1);
			isValid = 0;
		}
		
		/* If t2 is a num or char, we expect t1 to be of type t2. */
		else if(t2 == TNUMBER || t2 == TCHAR) {
			printErrorHeader("binary operator");
			error_type_mismatch(op, t1, t2);
			isValid = 0;
		}

		/* Otherwise both are invalid. */
		else {
			printErrorHeader("binary operator");
			error_type_mismatch(op, t1, "number/letter");
			error_type_mismatch(op, t2, "number/letter");
			isValid = 0;
		}
	}
		
	return isValid;
}

int NBinOp::checkBoolean() {
	int isValid = 1;

	int t1 = children[0]->getType();
	int t2 = children[1]->getType();

	switch(op) {
	case LEQU:
	case LNOTEQU:
	case LLTHAN:
	case LLTHANEQ:
	case LGTHAN:
	case LGTHANEQ: /* Arithmetic ops accept identical num/char. */
		isValid &= checkArithmetic(t1, t2);
		break;
	default: /* Everything else accepts identical bool, num, char */
		isValid &= checkPred(t1, t2);
		break;
	}

	/* Check that the operands (child nodes) are valid via Node. */
	isValid &= Node::check();
	
	return isValid;
}

int NBinOp::checkArithmetic(int t1, int t2) {
	if(t1 != TNUMBER && t1 != REFNUMBER && t1 != TCHAR && t1 != REFCHAR) {
		error_type_mismatch(op, t1, "number/letter");
		
		if(t2 != TNUMBER && t2 != REFNUMBER && t2 != TCHAR
				&& t2 != REFCHAR) {
			printErrorHeader("binary operator");
			error_type_mismatch(op, t2, "number/letter");	
		}

		return 0;
	}

	else if(!compareTypes(t1, t2)) {
		printErrorHeader("binary operator");
		error_type_mismatch(op, t2, t1);
		return 0;
	}

	return 1;
}

int NBinOp::checkPred(int t1, int t2) {
	if(t1 != BOOLEAN) {
		printErrorHeader("binary operator");
		error_type_mismatch(op, t1, BOOLEAN);
		return 0;
	}

	else if(t2 != BOOLEAN) {
		printErrorHeader("binary operator");
		error_type_mismatch(op, t2, BOOLEAN);
		return 0;
	}
	
	else {
		return 1;
	}
}

int NBinOp::isBoolean() {
	return (op == LEQU || op == LAND || op == LOR || op == LNOTEQU 
			|| op == LGTHAN || op == LGTHANEQ || op == LLTHAN 
			|| op == LLTHANEQ);
}
