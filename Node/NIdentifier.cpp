#ifndef ___NIDENTIFIER__
#define ___NIDENTIFIER__

#include "NIdentifier.hpp"
#include <string>

NIdentifier::NIdentifier() 
{ 
	name = "ID"; 
}                                           

string NIdentifier::getID() {
	return name;
}
//NIdentifier::NIdentifier(char *a) 
//{ 
//name = strdup(a);
//}

#endif
