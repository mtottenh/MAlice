/* Node describing a loop in the MAlice language. */

#ifndef _NLOOP_HPP
#define _NLOOP_HPP

#include "Node.hpp"
class NLoop : public Node {                                                
/*
 * The loop inherits type resolution from the base Node (does not have a type).
 * The check function also inherits from Node - the loop is valid if its child
 * statements are valid, with the added condition that the predicate actually
 * returns a Boolean.
 */
public:
	/*
	 * Constructor that takes a predicate node and a statement list node for the
	 * loop.
	 */
	NLoop(Node*, Node*);

	/*                                                                           
 	 * Checks that the node is semantically valid. Returns 1 if this is the      
 	 * case, 0 otherwise.                                                        
 	 */
	virtual int check();
	void accept(ASTVisitor*);
}; 

#endif
