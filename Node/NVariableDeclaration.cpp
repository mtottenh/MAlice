#include "NVariableDeclaration.hpp"
#include "TypeDefs.hpp"

/* 
 * To save creating a new enumeration of all the Types
 * we just extract the #defines from the auto genearted
 * y.tab.h file. But we need to define YYSTYPE in order
 
*/
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

int NVariableDeclaration::check() {
	int isValid = 1;

	/* Does the variable name already exist in current scope? */
	if(table->lookupCurrentScope(name) != NULL) {
		error_var_exists(name);
		isValid = 0;
	}

	/* Is it a keyword? */
	Node* ptr = table->lookup(name);
	if(ptr != NULL) {
		if(ptr->getType() == KEYWORD)  {
			error_keyword(name);
			isValid = 0;
		}
	}

	/* If we have children (i.e. array access bit expressions), check them. */
	isValid &= Node::check();

	return isValid;
}
