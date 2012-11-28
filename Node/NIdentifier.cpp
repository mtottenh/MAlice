#include "NIdentifier.hpp"
#include <string.h>
#include "TypeDefs.hpp"

/* Constructors. */

NIdentifier::NIdentifier() { 
	name = "ID";
	nodeType = IDENTIFIER;
	children.clear();
}
                                          
NIdentifier::NIdentifier(char *name) { 
	this->name = name;
	nodeType = IDENTIFIER;
}

NIdentifier::NIdentifier(string name) {
	this->name = name;
	nodeType = IDENTIFIER;
}

/* Public methods. */

string NIdentifier::getID() {
	return name;
}

/*  TODO Double check if this needs more too it or not */
int NIdentifier::check() {
	/* Does the identifier exist in the table? Valid if this is the case. */
	if (table->lookup(name) != NULL) {
		return 1;
	}

	return 0;
}

int NIdentifier::getType() {
	/* Invalid if we have an empty/null table. */
	if (table == NULL) {
		return INVALIDTYPE;
	}

	Node* variable = table->lookup(name);

	/* Does an entry exist in the symbol table? Return that type if so. */
	if ( variable != NULL) {
		return variable->getType();
	}

	/* Invalid type if no such entry exists. */
	else {
		return INVALIDTYPE;
	}
}

