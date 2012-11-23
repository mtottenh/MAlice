#ifndef ___NSTRING__
#define ___NSTRING__
#include "NString.hpp"
#include "TypeDefs.hpp"

NString::NString(string s) {
	this->type = resolveType();
	this->name = s;
}

NString::NString(char* s) {
	this->type = resolveType();
	this->name = strdup(s);
}

int NString::resolveType() {
	return TSTRING;
}
#endif
