#ifndef _NRETURN_H_
#define _NRETURN_H_

#include "Node.hpp"

class NReturn : public Node {
public:
	/*
	 * Type resolution and checking inherited from Node. Although the
	 * expression has a type, the actual return node does not.
	 */
	NReturn(Node *exp);
	virtual int resolveType();
};
#endif

