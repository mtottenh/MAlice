#ifndef _NPRINT_HPP
#define _NPRINT_HPP

#include "Node.hpp"
#include <string.h>
class NPrint : public Node {
private:
	/* Type resolution and validity checking inherited from Node. */
	char *message;
public:
        NPrint();
	NPrint(const char*); 
	NPrint(Node*);
	virtual int check();
};

#endif
