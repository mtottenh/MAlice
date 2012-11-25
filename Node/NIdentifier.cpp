#ifndef ___NIDENTIFIER__
#define ___NIDENTIFIER__

#include "NIdentifier.hpp"
#include <string.h>
#include "TypeDefs.hpp"
NIdentifier::NIdentifier() 
{ 
	name = "ID"; 
}                                           

string NIdentifier::getID() {
	return name;
}
NIdentifier::NIdentifier(char *name) { 
	this->name = strdup(name);
}
NIdentifier::NIdentifier(string name) {
	this->name = name;
}
/* Dummy check function just trying to get something working */
int NIdentifier::check() {
	if (table->lookup(name) != NULL) 
		return 1;
	return 0;
}

int NIdentifier::getType() {
	if (table == NULL) 
		return INVALIDTYPE;
	Node* variable = table->lookup(name);
	if ( variable != NULL)
		return variable->getType();
	else
		return INVALIDTYPE;
}
#endif

