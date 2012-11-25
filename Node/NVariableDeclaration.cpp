#include "NVariableDeclaration.hpp"
#include "TypeDefs.hpp"

NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type) {
	this->type = type;
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
}

NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type, Node *block) {
	this->type = getArrayType(type);
	children.push_back(block);
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
}

int NVariableDeclaration::getArrayType(int type) {
	switch(type) {
	case TNUMBER:
		return ARRNUMBER;
		break;
	case TCHAR:
		return ARRCHAR;
		break;
	case TSTRING:
		return ARRSTRING;
		break;
	default:
		return INVALIDTYPE;
		break;
	}
}
