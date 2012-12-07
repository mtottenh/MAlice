#ifndef _NNULLTOKEN_H_
#define _NNULLTOKEN_H_

#include "Node.hpp"
class NNullToken : public Node
{
public:
	NNullToken();
    virtual void accept(ASTVisitor *v);
};

#endif
