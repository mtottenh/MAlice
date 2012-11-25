#include "NVariableDeclaration.hpp"
#include "TypeDefs.hpp"
#include "../Errors/TypeMap.hpp"
NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type) {
	this->type = type;
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
}
/* Arrays*/
NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type, Node *block) {
	this->type = type;
	children.push_back(block);
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
}
