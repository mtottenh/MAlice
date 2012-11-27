#include "NCharLit.hpp"
#include "TypeDefs.hpp"

NCharLit::NCharLit() {
	name = "<Character Literal>";
	type = resolveType();
	nodeType = CHARLIT;
}

NCharLit::NCharLit(char *c) {
	this->character = *++c;
	name = character;
	type = resolveType();
	nodeType = CHARLIT;
}

int NCharLit::resolveType() {
	return TCHAR;
}

int NCharLit::check() {
	return 1;
}
