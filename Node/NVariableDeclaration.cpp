#include "NVariableDeclaration.hpp"
#include "TypeDefs.hpp"
#include "../Errors/TypeMap.hpp"
NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type) {
	this->type = type;
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
	cout << typemap_get(type) << endl;
	cout << typemap_get(this->type) << endl;
	cout << typemap_get(this->getType()) << endl;
	cout << typemap_get(this->resolveType()) << endl;
}

NVariableDeclaration::NVariableDeclaration(NIdentifier* id, int type, Node *block) {
	this->type = type;
	children.push_back(block);
	name = id->getID();
	delete(id);
	nodeType = VARDEC;
	cout << typemap_get(type);
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
