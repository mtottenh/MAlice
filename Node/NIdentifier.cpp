#ifndef ___NIDENTIFIER__
#define ___NIDENTIFIER__

#include "NIdentifier.hpp"
#include <string.h>

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
#endif

