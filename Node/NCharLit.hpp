#ifndef _NCHAR_HPP
#define _NCHAR_HPP

#include "Node.hpp"

class NCharLit : public Node { 
private:
	char character;
	virtual int resolveType();                                           
public:
	NCharLit();
	NCharLit(char *);
	virtual int check();
};  

#endif