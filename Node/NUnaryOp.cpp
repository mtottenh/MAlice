#include "NUnaryOp.hpp"
#include "TypeDefs.hpp"

NUnaryOp::NUnaryOp(int op, Node* exp) { 
	name = "UnaryOp"; 
	this->op = op;
	children.push_back(exp);
	nodeType = UNARYOP;
}

int NUnaryOp::resolveType() {
	int childType = children[0]->getType();

	/* Is the type of the expression a number? */
	if(childType == TNUMBER || childType == REFNUMBER) {
		return TNUMBER;
	}

	/* Or is it a boolean? */
	else if(childType == BOOLEAN) {
		return BOOLEAN;
	}
	
	/* Otherwise, invalid. */
	else {
		return INVALIDTYPE;
	}
}

/* TODO refactor to make the code clearer,
 */
int NUnaryOp::check() {
	int isValid = 1;

	this->type = resolveType();

	/* Do we have a unary boolean expression? (i.e. NOT) */
	if(op == LNOT) {
		return checkBoolean();
	}

	/* Valid if the type is a number and the children are valid. */
	isValid &= Node::check();

	/* Is the type invalid? */
	if(type == INVALIDTYPE) {
		printErrorHeader("unary operator");
		error_type_mismatch(op, children[0]->getType(), TNUMBER);
		isValid = 0;
	}

	return isValid;
}

int NUnaryOp::checkBoolean() {
	if(type != BOOLEAN) {
		printErrorHeader("unary operator");
		error_type_mismatch(op, type, BOOLEAN);
		return 0;
	}

	return 1;
}
