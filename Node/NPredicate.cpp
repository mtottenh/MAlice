#ifndef ___NPREDICATE__
#define ___NPREDICATE__

#include "NPredicate.hpp"
#include "TypeDefs.hpp"

NPredicate::NPredicate(Node *left, int op, Node* right) {
	name = "Predicate";
	this->op = op;
	this->type = BOOLEAN;
	children.push_back(left);
	children.push_back(right);
}

NPredicate::NPredicate(int op, Node* predicate) {
	this->op = op;
	this->type = BOOLEAN;
	name = "Predicate";
	children.push_back(predicate);
}

int NPredicate::resolveType() {
	return BOOLEAN;
}

int NPredicate::check() {
	int isValid = 1;
	int numOperands = children.size();

	
	/* If the operator is unary (LNOT), we expect 1 operand. */
	if(op == LNOT) {
		isValid &= compareArity(1, numOperands);
	}

	/* Otherwise we have a binary function and expect 2 operands. */
	else {
		isValid &= compareArity(2, numOperands);
	}

	/*
	 * If we're still valid, we need to ensure the types of the operands
	 * are correct.
	 */
	if(isValid) {
		int t1 = children[0]->getType();
		int t2;

		switch(op) {
		case LNOT: /* NOT accepts a boolean. */
			isValid &= checkLNOT(t1);
			break;
		case LEQU: /* LEQU (e.g. x == y) accepts identical num/char. */
			t2 = children[1]->getType();
			isValid &= checkLEQU(t1, t2);
			break;
		default: /* Everything else accepts identical bool, num, char */
			t2 = children[1]->getType();
			isValid &= checkPred(t1, t2);
			break;
		}
	}

	/* Check that the operands (child nodes) are valid via Node. */
	isValid &= Node::check();
	
	return isValid;
}

int NPredicate::compareArity(int expected, int given) {
	if(expected != given) {
		error_num_args(op, given, expected);
		return 0;
	}
	else {
		return 1;
	}
}

int NPredicate::checkLNOT(int type) {
	if(type != BOOLEAN) {
		error_type_mismatch(op, type, BOOLEAN);
		return 0;
	}

	return 1;
}

int NPredicate::checkDASH(int type) {
	if(type != TNUMBER) {
		error_type_mismatch(op, type, TNUMBER);
		return 0;
	}

	return 1;
}

int NPredicate::checkLEQU(int t1, int t2) {
	if(t1 != TNUMBER && t1 != TCHAR) {
		error_type_mismatch(op, t1, "number/letter");
		
		if(t2 != TNUMBER && t2 != TCHAR) {
			error_type_mismatch(op, t2, "number/letter");	
		}

		return 0;
	}

	else if(t2 != t1) {
		error_type_mismatch(op, t2, t1);
		return 0;
	}

	return 1;
}

int NPredicate::checkPred(int t1, int t2) {
	if(t1 != BOOLEAN && t1 != TNUMBER && t1 != TCHAR) {
		cout << "LHS BROKE";
		error_type_mismatch(op, t1, "number/letter/boolean");
		
		if(t2 != BOOLEAN && t2 != TNUMBER && t2 != TCHAR) {
			cout << "RHS BROKE";
			error_type_mismatch(op, t2, "number/letter/boolean");
		}

		return 0;
	}
	
	else if (t2 != t1) {
		error_type_mismatch(op, t2, t1);
		return 0;
	}

	return 1;
}

#endif
