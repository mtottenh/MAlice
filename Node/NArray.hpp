#ifndef _NARRAY_H_
#define _NARRAY_H_

#include "Node.hpp"
#include "NIdentifier.hpp"

class NArray : public Node {

public:
	NArray();
	NArray(NIdentifier*,Node*);
};
#endif
