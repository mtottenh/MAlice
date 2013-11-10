#include "../CodeGeneration/ASTVisitor.hpp"

#include "NParamBlock.hpp"
#include "TypeDefs.hpp"
NParamBlock::NParamBlock(Node *node)
{
	name = "NParamBlock";
	children.push_back(node);
	nodeType = PARAMBLOCK;
}


void NParamBlock::accept(ASTVisitor *v) {
    v->visit(this);
}
