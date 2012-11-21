#include "NVariableDeclaration.hpp"
#include "../SymbolTable.hpp"

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

int NVariableDeclaration::check(SymbolTable* table) {
	int isValid = 1;

	/* Does the variable name already exist in current scope? */
	if(table.lookupCurrentScope(name) != NULL) {
		error_var_exists(name);
		isValid = 0;
	}

	/* Is it a keyword? */
	SymbolTable* ptr;
	if(ptr = table.lookup(name) != NULL) {
		if(ptr->getType() == KEYWORD)  {
			error_keyword(name);
			isValid = 0;
		}
	}

	/* If we have children (i.e. array access bit expressions), check them. */
	valid = Node::check(table);

	return isValid;
}
