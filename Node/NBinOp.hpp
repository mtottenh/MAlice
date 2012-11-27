#ifndef _NBINOP_HPP_
#define _NBINOP_HPP_

#include "Node.hpp"

class NBinOp : public Node {
private:
	/* 
	 * Check function for a binary boolean operator. Returns 1 if the 
	 * boolean expression is semantically correct, 0 otherwise.
	 */
	int checkBoolean();

	/* 
	 * Checks that two types are valid in an arithmetic binary operator.
	 * Returns SUCCESS if valid, FAILURE otherwise.
	 */
	int checkArithmetic(int, int);

	/* 
	 * Checks that two types are valid in an boolean binary operator.
	 * Returns SUCCESS if valid, FAILURE otherwise.
	 */
	int checkPred(int, int);

	/* 
	 * Returns whether the node operator (+, && etc) is a boolean operator.
	 */
	int isBoolean();
protected:
	/* Field containing the integer value of the node's binary operator. */
	int op;

	/* 
	 * Resolves the type of the node, returning an integer representation of
	 * the type.
	 */
	virtual int resolveType() ;
public:
	/* Constructor taking a left node, right node and operator. */
	NBinOp(Node*, Node*, int);

	/* Returns an int representing the data type of the node. */
	virtual int getType();

	/*
	 * Checks whether the node is semantically valid. Returns 1 if correct,
	 * 0 otherwise.
	 */
	virtual int check();
};

#endif
