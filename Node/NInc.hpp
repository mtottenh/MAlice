#ifndef _NINC_HPP
#define _NINC_HPP

#include "Node.hpp"

class NInc  : public Node {
public:
	NInc(Node *child) { children.push_back(child);}
};   

#endif
