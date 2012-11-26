#ifndef _NSTRINGLIT_H_
#define _NSTRINGLIT_H_

#include <string>
#include "Node.hpp"
class NStringLit : public Node
{
public:
	NStringLit(char*);
	NStringLit(string);
	string literal;
	virtual int resolveType();
};

#endif
