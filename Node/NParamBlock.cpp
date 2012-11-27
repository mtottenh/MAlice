#include "NParamBlock.hpp"

NParamBlock::NParamBlock(Node *node)
{
	name = "NParamBlock";
	children.push_back(node);
}
