#ifndef _NARRAYACCESS_H_
#define _NARRAYACCESS_H_

#include "Node.hpp"
#include "NIdentifier.hpp"

class NArrayAccess : public Node
{
public:
	NArrayAccess(NIdentifier*, Node*);
};
#endif
