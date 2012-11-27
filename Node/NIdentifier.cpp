#ifndef ___NIDENTIFIER__
#define ___NIDENTIFIER__

#include "NIdentifier.hpp"
#include <string.h>
#include "TypeDefs.hpp"
NIdentifier::NIdentifier() 
{ 
	name = "ID";
	nodeType = IDENTIFIER;
}                                           

string NIdentifier::getID() {
	return name;
}
NIdentifier::NIdentifier(char *name) { 
	this->name = name;
	nodeType = IDENTIFIER;
}
NIdentifier::NIdentifier(string name) {
	this->name = name;
	nodeType = IDENTIFIER;
}
/* Checking an identifier is juts checking whether it is declared in scope 
 *  TODO Double check if this needs more too it or not
 */
int NIdentifier::check() {
	if (table->lookup(name) != NULL) 
		return 1;
	return 0;
}

int NIdentifier::getType() {
/* TODOO remove table === NULL checkk */
	if (table == NULL) 
		return INVALIDTYPE;
	Node* variable = table->lookup(name);
	if ( variable != NULL)
		return variable->getType();
	else
		return INVALIDTYPE;
}
#endif

