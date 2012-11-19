#ifndef _NUNARYOP_H_
#define _NUNARYOP_H_
#include "Node.hpp"
/* TODO : Put this implementation into a .cpp file */
class NUnaryOp : public Node { 
public:
	NUnaryOp(int opperator, Node* child) { name = "Unary Op"; children.push_back(child);};
};
#endif
