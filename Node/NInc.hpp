/* Node representing a MAlice decrement operation. */

#ifndef _NINC_HPP
#define _NINC_HPP

#include "Node.hpp"
#include "NIdentifier.hpp"

class NInc : public NIdentifier {
protected:
	/* Resolves the type of the node and returns it's int representation */
	virtual int resolveType();
public:
	/* Constructor that takes a child node (what is to be incremented). */
	NInc(Node *child);

	/*                                                                           
 	 * Checks that the node is semantically valid. Returns 1 if this is the      
 	 * case, 0 otherwise.                                                        
 	 */
	virtual int check();
};   

#endif
