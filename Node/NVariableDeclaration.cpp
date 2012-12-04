#include "NVariableDeclaration.hpp"
#include "TypeDefs.hpp"
#include "../CodeGeneration/ASTVisitor.hpp"

NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type) {
	this->type = type;
	name = id->getID();
	this->setLocation(id->getLocation());
	delete(id);
	nodeType = VARDEC;
}
/* Arrays*/
NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type, Node *block) {
	this->type = getArrayType(type);
	children.push_back(block);
	name = id->getID();
	this->setLocation(id->getLocation());
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

void NVariableDeclaration::accept(ASTVisitor* v) {
    cout << "Node: ";
    v->visit(this);
}
