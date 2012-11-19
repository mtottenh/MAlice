#ifndef _NPREDICATE_HPP_
#define _NPREDICATE_HPP_

#include "Node.hpp"

class NPredicate : public Node {
private:
	int op;
public:
	NPredicate(Node *Left, int opperator, Node* Right){
		name = "Predicate";
		op = opperator;
		children.push_back(Left);
		children.push_back(Right);
	};
	NPredicate(int opperator, Node* Predicate){
		op = opperator;
		name = "Predicate";
		children.push_back(Predicate);
	}
	
};
#endif
