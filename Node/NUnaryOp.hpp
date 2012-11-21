#ifndef _NUNARYOP_H_
#define _NUNARYOP_H_
#include "Node.hpp"

class NUnaryOp : public Node { 
public:
	NUnaryOp(int op, Node*);
};

#endif
