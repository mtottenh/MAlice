#ifndef _NPREDICATE_HPP_
#define _NPREDICATE_HPP_

#include "Node.hpp"

class NPredicate : public Node {
private:
	int op;
	virtual int resolveType();
	int compareArity(int, int);
	int checkLNOT(int);
	int checkLEQU(int, int);
	int checkDASH(int);
	int checkPred(int, int);
public:
	NPredicate(Node*, int, Node*);
	NPredicate(int, Node*);	
	virtual int check();
};

#endif
