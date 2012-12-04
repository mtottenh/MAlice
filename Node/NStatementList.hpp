#ifndef _NSTATEMENTList_HPP_
#define _NSTATEMENTList_HPP_

#include "Node.hpp"

class NStatementList : public Node {
/* Statement list has no type, but check function needed. */
public:
        NStatementList();
	NStatementList(Node* , Node*);
	virtual int check();
    void accept(ASTVisitor* );
}; 

#endif
