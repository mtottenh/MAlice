/* Node containing a MAlice conditional expression (if/then/else) */

#ifndef _NCONDITIONAL_HPP
#define _NCONDITIONAL_HPP

#include "Node.hpp"

class NConditional : public Node {
protected:
	/* Resolves the type of the node and returns its int representation. */
	virtual int resolveType();
public:
	/*
	 * Constructor taking a predicate node, left node (then) and right node
	 * (else/else if/end if).
	 */
	NConditional(Node*, Node*, Node*);

	/*
	 * Checks whether the node is semantically valid. Returns 1 if correct,
	 * 0 otherwise.
	 */
	virtual int check();
}; 

#endif
