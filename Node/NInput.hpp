#ifndef _NINPUT_HPP_
#define _NINPUT_HPP_

#include "Node.hpp"

class NInput : public Node {                                               
public:
	NInput(Node*);
	virtual int check();
};  

#endif
