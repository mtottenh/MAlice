#include "NParamDeclarationBlock.hpp"
#include "TypeDefs.hpp"
NParamDeclarationBlock::NParamDeclarationBlock(Node* n)
{
	name = "ParamDeclarationBlock";
	children.push_back(n);
	nodeType = PARAMDEC;
}
