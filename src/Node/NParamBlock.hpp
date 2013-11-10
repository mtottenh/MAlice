#ifndef _NPARAMBLOCK_H_
#define _NPARAMBLOCK_H_

#include "Node.hpp"

class NParamBlock : public Node
{
/*
 * A parameter block doesn't have a type, so its type is inherited from Node. 
 * A parameter block is valid if its children are valid, so inherit from Node.
 */
public:
	NParamBlock(Node*);
    virtual void accept(ASTVisitor*);
};

#endif
