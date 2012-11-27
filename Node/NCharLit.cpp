#include "NCharLit.hpp"
#include "TypeDefs.hpp"

NCharLit::NCharLit() {
	name = "Char";
	type = resolveType();
	nodeType = CHARLIT;
}

NCharLit::NCharLit(char *c) {
	name = "Char";
	this->character = *++c;
	type = resolveType();
	nodeType = CHARLIT;
}

int NCharLit::resolveType() {
	return TCHAR;
}

int NCharLit::check() {
	return 1;
}
