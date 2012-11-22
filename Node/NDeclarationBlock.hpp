#ifndef _NDECLARATIONBLOCK_HPP_
#define _NDECLARATIONBLOCK_HPP_

#include "Node.hpp"
class NDeclarationBlock : public Node {  
public:
	/*
	 * Type resolution and checking are inherited from Node superclass.
	 * Small addition to check for 'hatta' entry point!
	 */	
	NDeclarationBlock();
	NDeclarationBlock(Node *child);
	virtual int check();
};  

#endif
