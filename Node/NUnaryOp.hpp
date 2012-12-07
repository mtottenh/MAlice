#ifndef _NUNARYOP_H_
#define _NUNARYOP_H_
#include "Node.hpp"

class NUnaryOp : public Node { 
private:
	int op;
	virtual int resolveType();
	int checkBoolean();
protected:
	/* Calculate the weight of the node. */
	virtual int calculateWeight();
public:
	NUnaryOp(int, Node*);
	virtual int check();
    virtual void accept(ASTVisitor*);
};

#endif
