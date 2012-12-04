#include "NBinOp.hpp"
#include "TypeDefs.hpp"

/* Constructors */
NBinOp::NBinOp(Node* left, Node* right, int op) {
	this->op = op;
	this->name = "Binary Operator";
	this->nodeType = BINOP;
	this->weight = -1;
	children.push_back(left);
	children.push_back(right);
}

/* Public methods */

int NBinOp::getType() {
	return resolveType();
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

/* Protected methods. */

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

int NBinOp::calculateWeight() {
	/* 
	 * The weight of the binary operator is the minimum weight of evaluating
	 * the LHS or RHS.
	 */
	Node* lhsWeight = children[0]->getWeight();
	Node* rhsWeight = children[1]->getWeight();

	/* Cost of evaluating rhs first, plus 1 to store the value of rhs. */
	int cost1 = max(lhsWeight, rhsWeight + 1);

	/* Cost of evaluating rhs lirst, plus 1 to store the value of lhs. */
	int cost2 = max(lhsWeight + 1, rhsWeight);

	return min(cost1, cost2);
}

/* Private methods. */

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
	/* Is t1 a number or character? (or a reference to one?) */
	if(t1 != TNUMBER && t1 != REFNUMBER && t1 != TCHAR && t1 != REFCHAR) {
		error_type_mismatch(op, t1, "number/letter");
		
		/* Okay, what about t2? */
		if(t2 != TNUMBER && t2 != REFNUMBER && t2 != TCHAR
				&& t2 != REFCHAR) {
			printErrorHeader("binary operator");
			error_type_mismatch(op, t2, "number/letter");	
		}

		return FAILURE;
	}

	/* 
	 * We now know that t1 and t2 are of the correct type, but are they of
	 * the same type?
	 */
	else if(!compareTypes(t1, t2)) {
		printErrorHeader("binary operator");
		error_type_mismatch(op, t2, t1);
		return FAILURE;
	}

	return SUCCESS;
}

int NBinOp::checkPred(int t1, int t2) {
	/* Is t1 a boolean? */
	if(t1 != BOOLEAN) {
		printErrorHeader("binary operator");
		error_type_mismatch(op, t1, BOOLEAN);
		return FAILURE;
	}

	/* Is t2 a boolean? */
	else if(t2 != BOOLEAN) {
		printErrorHeader("binary operator");
		error_type_mismatch(op, t2, BOOLEAN);
		return FAILURE;
	}
	
	/* Both are booleans, great success! */
	else {
		return SUCCESS;
	}
}

int NBinOp::isBoolean() {
	/* Is the operator one that accepts booleans? */
	return (op == LEQU || op == LAND || op == LOR || op == LNOTEQU 
			|| op == LGTHAN || op == LGTHANEQ || op == LLTHAN 
			|| op == LLTHANEQ);
}
