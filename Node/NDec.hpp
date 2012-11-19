#ifndef _NDEC_HPP
#define _NDEC_HPP

#include "Node.hpp"

class NDec : public Node {
public:
	NDec(Node *child) { children.push_back(child);}
}; 

#endif
