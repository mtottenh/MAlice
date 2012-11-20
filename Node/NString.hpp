#ifndef _NSTRING_H_
#define _NSTRING_H

#include "Node.hpp"
#include <string.h>

class NString : public Node {
public:
	NString(string s) { name = s; };
	NString(char* s) { name = strdup(s);};
};  

#endif
