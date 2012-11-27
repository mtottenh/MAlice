#ifndef _NNULLTOKEN_H_
#define _NNULLTOKEN_H_

#include "NNullToken.hpp"
/* TODO remove implementation from header
 * even though this is a dummy class
 * we should keep to the project conventions
 * of keeping implementations separate from
 * the headers
 */
class NNullToken : public Node
{
public:
	NNullToken() { name = "NNullToken";}
};

#endif
