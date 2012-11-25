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
		return REFNUMBER;
		break;
	case TCHAR:
		return REFCHAR;
		break;
	case TSTRING:
		return REFSTRING;
		break;
	default:
		return INVALIDTYPE;
		break;
	}
}
