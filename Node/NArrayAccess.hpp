/* Node class for a MAlice array access. */

#ifndef _NARRAYACCESS_H_
#define _NARRAYACCESS_H_

#include "Node.hpp"
#include "NIdentifier.hpp"

class NArrayAccess : public Node {
protected:
	/* Node representing the identifier of the array. */
	NIdentifier* id;

	/* Node representing the expression of the array access. */
	Node* indexNode;

	/*
	 * Resolves the type of the array access, e.g. an access on a letter
	 * array will resolve to type letter.
	 */
	virtual int resolveType();
public:
	NArrayAccess(NIdentifier*, Node*);

	/* Returns an int representing the type of the node. */
	virtual int getType();

	/*
	 * Checks that the code the node represents is semantically correct.
	 * Returns SUCCESS if correct, FAILURE otherwise.
	 */
	virtual int check();
};

#endif
