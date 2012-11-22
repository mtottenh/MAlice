#include "NDeclarationBlock.hpp"

NDeclarationBlock::NDeclarationBlock() {
	this->type = resolveType();
}

NDeclarationBlock::NDeclarationBlock(Node* child) {
	this->type = resolveType();
	children.push_back(child);
}
