#include "NCodeBlock.hpp"

NCodeBlock::NCodeBlock(NDeclarationBlock *dec, NStatementList *stat)
{
	children.push_back(dec);
	children.push_back(stat);
}

NCodeBlock::NCodeBlock(NStatementList *stat)
{
	children.push_back(stat);
}

NCodeBlock::NCodeBlock() {}
