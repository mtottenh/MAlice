#ifndef _NINC_HPP
#define _NINC_HPP

#include "Node.hpp"
#include "NIdentifier.hpp"

class NInc : public NIdentifier {
private:
	virtual int resolveType();
public:
	NInc(Node *child);
	virtual int check();
};   

#endif
