#ifndef _NPRINT_HPP
#define _NPRINT_HPP

#include "Node.hpp"
#include <string.h>
class NPrint : public Node {
private:
	char *message;
public:
        NPrint();
	NPrint(const char *msg) { message = strdup(msg); name = "print";}
	NPrint(Node *child){ name = "print";children.push_back(child); }
};

#endif
