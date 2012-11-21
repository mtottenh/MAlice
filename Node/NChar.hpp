#ifndef _NCHAR_HPP
#define _NCHAR_HPP

#include "Node.hpp"

class NChar : public Node { 
private:
	char character;                                           
public:
        NChar();
	NChar(char *);
};  

#endif
