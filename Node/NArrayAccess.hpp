#ifndef _NARRAYACCESS_H_
#define _NARRAYACCESS_H_

#include "Node.hpp"
#include "NIdentifier.hpp"

class NArrayAccess : public Node
{
private:
	NIdentifier* id;
	Node* indexNode;
public:
	NArrayAccess(NIdentifier*, Node*);
	virtual int check();
};


#endif
