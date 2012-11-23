#ifndef _NSTRING_H_
#define _NSTRING_H_

#include "Node.hpp"
#include <string.h>

class NString : public Node {
private:
	virtual int resolveType();
public:
	NString(string);
	NString(char*);
};  

#endif
