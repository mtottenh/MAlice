#include "NDeclarationBlock.hpp"
#include "TypeDefs.hpp"

/* Constructors. */

NDeclarationBlock::NDeclarationBlock() {
	this->name = "Declaration Block";
	this->nodeType = DECLARATIONBLOCK;
	this->isRootNode = false;
}

NDeclarationBlock::NDeclarationBlock(Node* child) {
	children.push_back(child);
	this->name = "Declaration Block";
	this->nodeType = DECLARATIONBLOCK;
	this->isRootNode = false;
}

/* Public methods. */

int NDeclarationBlock::check() {
	/* 
	 * Check for 'hatta' entry point, if this is the topmost root node.
	 * Delegate to checkRoot().
	 */
	if(isRootNode) {
		return checkRoot();
	}

	/* Otherwise, check the children via generic check function. */
	return Node::check();
}

/* Private methods. */

int NDeclarationBlock::checkRoot() {
	int isValid = 1;

	/* Look for hatta in the symbol table. */
	Node* nodePtr = table->lookup("hatta");

	/* Hatta, are you thereeee? */
	if(nodePtr == NULL) {
		error_no_entry();
		isValid = 0;
	}

	/* Hatta is there! But it's not a procedure. Sad face :( */
	else if (nodePtr->getNodeType() != PROCEDURE) {
		error_no_entry();
		isValid = 0;
	}

	return isValid;
}
