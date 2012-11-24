#include "NVariableDeclaration.hpp"
#include "TypeDefs.hpp"
#include "../Errors/TypeMap.hpp"
NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type) {
	this->type = type;
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
}

NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type, Node *block) {
	this->type = type;
	children.push_back(block);
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
}

int NVariableDeclaration::check() {
	int isValid = 1;
	/*THIS CHECK IS ALREADY PERFORMED IN THE SYMTABLE GENERATOR*/
	/* Does the variable name already exist in current scope? */
/*	if(table->lookupCurrentScope(name) != NULL) {
		error_var_exists(name);
		isValid = 0;
	}
*/
	/* If we have children (i.e. array access bit expressions), check them. */
	isValid &= Node::check();

	return isValid;
}
