#ifndef _NDEC_HPP
#define _NDEC_HPP

#include "Node.hpp"
#include "NIdentifier.hpp"

class NDec : public NIdentifier {
private:
	virtual int resolveType();
public:
	NDec(Node *child); 
	virtual int check();
}; 

#endif
