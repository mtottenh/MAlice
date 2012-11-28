#include "NCodeBlock.hpp"
#include "TypeDefs.hpp"

/* Code blocks consist of a statement list and an optional declaration list. */

/* Constructors. */

NCodeBlock::NCodeBlock(Node *dec, Node *stat) {
	/* Set fields and push declaration block, along with statement list. */
	this->name = "CodeBlock";
	children.push_back(dec);
	children.push_back(stat);
	this->nodeType = CODEBLOCK;
}

NCodeBlock::NCodeBlock(Node *stat) {
	/* Set fields, push statement list. */
	this->name = "CodeBlock";
	children.push_back(stat);
	this->nodeType = CODEBLOCK;
}

NCodeBlock::NCodeBlock() {
	this->name = "CodeBlock";
}
