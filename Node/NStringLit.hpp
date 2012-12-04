#ifndef _NSTRINGLIT_H_
#define _NSTRINGLIT_H_

#include "Node.hpp"

class NStringLit : public Node
{
public:
	NStringLit(char*);
	NStringLit(string);
	string literal;
	virtual int resolveType();
    void accept(ASTVisitor*);
};

#endif
