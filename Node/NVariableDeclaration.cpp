#include "NVariableDeclaration.hpp"

NVariableDeclaration::NVariableDeclaration() {
	name = "Declaration";
}

NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type) {
	this->type = type;
	name = id->getID();
	delete(id);
}

NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type, Node *block) {
	this->type = type;
	children.push_back(block);
	name = id->getID();
	delete(id);
}	
