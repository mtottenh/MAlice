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
	NLoop(Node* predicate, Node* statementList);
	virtual int check(); 
}; 

#endif