#ifndef _NPARAMDECLARATIONBLOCK_H_
#define _NPARAMDECLARATIONBLOCK_H_

#include "Node.hpp"

class NParamDeclarationBlock : public Node
{
/*
 * Has no type and the check function returns true if its children are valid.
 * Inherit from Node.
 */
public:
	NParamDeclarationBlock();
	NParamDeclarationBlock(Node*);
    virtual void accept(ASTVisitor*);
};

#endif
