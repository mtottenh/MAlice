/* Node containing a character literal. */

#ifndef _NCHAR_HPP
#define _NCHAR_HPP

#include "Node.hpp"

class NCharLit : public Node { 
private:
	/* Field that stores the character literal of the node. */
	char character;

	/* Resolves the type of the node and returns it. */
	virtual int resolveType();                                           
public:
	/* Constructor passed no character literal value. */
	NCharLit();

	/* Constructor taking a character literal value. */
	NCharLit(char *);

	/*
	 * Checks if the node is semantically valid. Returns 1 if it is valid,
	 * 0 otherwise.
	 */
	virtual int check();
	void accept(ASTVisitor*);
};  

#endif
