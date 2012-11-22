#ifndef _NCONDITIONAL_HPP
#define _NCONDITIONAL_HPP

#include "Node.hpp"

class NConditional : public Node {
private:
	virtual int resolveType();
public:
	NConditional(Node*, Node*, Node*);
	virtual int check();
}; 

#endif
