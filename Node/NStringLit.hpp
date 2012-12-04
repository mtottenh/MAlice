#ifndef _NSTRINGLIT_H_
#define _NSTRINGLIT_H_

#include "Node.hpp"

class NStringLit : public Node {
protected:
	/* Calculate the weight of the node. */
	virtual int calculateWeight();
public:
	NStringLit(char*);
	NStringLit(string);
	string literal;
	virtual int resolveType();
    void accept(ASTVisitor*);
};

#endif
