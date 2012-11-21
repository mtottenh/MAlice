#ifndef _NPREDICATE_HPP_
#define _NPREDICATE_HPP_

#include "Node.hpp"

class NPredicate : public Node {
private:
	int op;
public:
	NPredicate(Node*, int, Node*);
	NPredicate(int, Node*);	
};
#endif
