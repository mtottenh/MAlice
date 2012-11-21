#ifndef _NINC_HPP
#define _NINC_HPP

#include "Node.hpp"
#include "NIdentifier.hpp"

class NInc  : public NIdentifier {
public:
	NInc(Node *child) {name = "Incrment"; children.push_back(child);}
};   

#endif
