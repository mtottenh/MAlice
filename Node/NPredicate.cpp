#ifndef ___NPREDICATE__
#define ___NPREDICATE__

#include "NPredicate.hpp"

NPredicate::NPredicate(Node *left, int op, Node* right){
		name = "Predicate";
		this->op = op;
		children.push_back(left);
		children.push_back(right);
}

NPredicate::NPredicate(int op, Node* Predicate){
		this->op = op;
		name = "Predicate";
		children.push_back(Predicate);
}

#endif
