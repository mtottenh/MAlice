#ifndef _NUNARYOP_H_
#define _NUNARYOP_H_
#include "Node.hpp"

class NUnaryOp : public Node { 
private:
	int op;
public:
	NUnaryOp(int, Node*);
};

#endif
