#ifndef _NBINOP_HPP_
#define _NBINOP_HPP_

#include "Node.hpp"

typedef enum
{
	BAND, BOR, BXOR, BPLUS, BMINUS, BMULT, BDIV, BMOD
} BinaryOperator;

class NBinOp : public Node {
public:
		BinaryOperator binOp;
        NBinOp(Node*, Node*, BinaryOperator);
};

#endif
