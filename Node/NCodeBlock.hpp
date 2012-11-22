#ifndef _NCODEBLOCK_H_
#define _NCODEBLOCK_H_

#include "Node.hpp"
#include "NStatementList.hpp"
#include "NDeclarationBlock.hpp"

class NCodeBlock : public Node
{
public:
	NCodeBlock(NDeclarationBlock*, NStatementList*);
	NCodeBlock(NStatementList*);
	NCodeBlock();
};

#endif
