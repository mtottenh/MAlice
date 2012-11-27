#include "NParamBlock.hpp"
#include "TypeDefs.hpp"
NParamBlock::NParamBlock(Node *node)
{
	name = "NParamBlock";
	children.push_back(node);
	nodeType = PARAMBLOCK;
}
