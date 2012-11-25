#ifndef _NMETHODCALL_HPP_
#define _NMETHODCALL_HPP_

#include "Node.hpp"
#include "NIdentifier.hpp"
/* Fix this class - could also be expression?*/                                  
class NMethodCall : public Node {                                          
public:
	virtual int resolveType();
	int checkParams(Node*);
public:
	NMethodCall();
	NMethodCall(NIdentifier*,Node*);
	NMethodCall(NIdentifier*);
	virtual int check();
	int getType() const;
}; 

#endif
