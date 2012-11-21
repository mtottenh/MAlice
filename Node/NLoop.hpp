#ifndef _NLOOP_HPP
#define _NLOOP_HPP

#include "Node.hpp"
class NLoop : public Node {                                                
public:
	NLoop(Node* predicate, Node* statementList); 
}; 

#endif
