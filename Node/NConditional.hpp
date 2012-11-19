#ifndef _NCONDITIONAL_HPP
#define _NCONDITIONAL_HPP

#include "Node.hpp"

class NConditional : public Node {
public:
	NConditional(Node* Predicate, Node* Left, Node* Right){
		name = "Conditional";
		children.push_back(Predicate);
		children.push_back(Left);
		children.push_back(Right);
	};
}; 

#endif
