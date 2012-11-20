#ifndef _NSTATEMENTList_HPP_
#define _NSTATEMENTList_HPP_

#include "Node.hpp"

class NStatementList : public Node {
public:
        NStatementList() { name = "StatList"; }
	NStatementList(Node* , Node*);
}; 

#endif
