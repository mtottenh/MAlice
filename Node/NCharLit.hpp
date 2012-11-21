#ifndef _NCHAR_HPP
#define _NCHAR_HPP

#include "Node.hpp"

class NCharLit : public Node { 
private:
	char character;                                           
public:
	NCharLit();
	NCharLit(char *);
};  

#endif
