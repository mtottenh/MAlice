#include "NCodeBlock.hpp"
#include "TypeDefs.hpp"
/* BAD MITCHELL!! Please use Node *'s as the paramaters,
 * it doesn't matter that you don't know what the exact
 * type of the nodes are as you are just pushing them into
 * the children deque. If you use specific types it
 * breaks other bits of code
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

NCodeBlock::NCodeBlock() {}
