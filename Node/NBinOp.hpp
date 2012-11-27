#ifndef _NBINOP_HPP_
#define _NBINOP_HPP_

#include "Node.hpp"

class NBinOp : public Node {
private:
	int checkBoolean();
	int checkArithmetic(int, int);
	int checkPred(int, int);
	int isBoolean();
protected:
	int op;
	virtual int resolveType() ;
public:
        NBinOp(Node*, Node*, int);
	virtual int check();
	virtual int getType();
};

#endif
