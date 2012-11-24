#ifndef _NBINOP_HPP_
#define _NBINOP_HPP_

#include "Node.hpp"

class NBinOp : public Node {
private:
	int op;
	virtual int resolveType() const;
public:
        NBinOp(Node*, Node*, int);
	virtual int check();
	virtual int getType() const;
};

#endif
