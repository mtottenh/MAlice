#ifndef ___NLOOP__
#define ___NLOOP__
#include "NLoop.hpp"

NLoop::NLoop(Node* predicate, Node* statementList) 
{
	children.push_back(statementList);
	children.push_back(predicate);
}

#endif
