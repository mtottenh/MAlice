/* Node representing a MAlice 'end if' statement. */

#ifndef _NENDIF_H_
#define _NENDIF_H_

#include "Node.hpp"

class NEndIf : public Node
{
public:
	NEndIf();
    virtual void accept(ASTVisitor*);
};

#endif
