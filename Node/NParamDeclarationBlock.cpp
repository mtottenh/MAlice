#include "NParamDeclarationBlock.hpp"

NParamDeclarationBlock::NParamDeclarationBlock(Node* n)
{
	name = "ParamDeclarationBlock";
	children.push_back(n);
}
