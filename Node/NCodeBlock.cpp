#include "NCodeBlock.hpp"
#include "TypeDefs.hpp"

/* Code blocks consist of a statement list and 
 * an optional declaration list
 */
NCodeBlock::NCodeBlock(Node *dec, Node *stat)
{
	name = "CodeBlock";
	children.push_back(dec);
	children.push_back(stat);
	nodeType = CODEBLOCK;
}

NCodeBlock::NCodeBlock(Node *stat)
{
	name = "CodeBlock";
	children.push_back(stat);
	nodeType = CODEBLOCK;
}

NCodeBlock::NCodeBlock() 
{
	name = "CodeBlock";
}
