/* Node representing a MAlice decrement operation. */

#ifndef _NDEC_HPP
#define _NDEC_HPP

#include "Node.hpp"
#include "NIdentifier.hpp"

class NDec : public NIdentifier {
protected:
	/* Resolves the type of the node and returns it's int representation */
	virtual int resolveType();
public:
	/* Constructor that takes a child node (what is to be decremented). */
	NDec(Node *child);
 
	/*
	 * Checks that the node is semantically valid. Returns 1 if this is the
	 * case, 0 otherwise.
	 */
	virtual int check();
}; 

#endif
