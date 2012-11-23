#include "NCharLit.hpp"
#include "TypeDefs.hpp"

NCharLit::NCharLit() {
	name = "Char";
	type = resolveType();
}

NCharLit::NCharLit(char *c) {
	name = "Char";
	this->character = *++c;
	type = resolveType();
}

int NCharLit::resolveType() {
	return TCHAR;
}

int NCharLit::check() {
	return 1;
}
