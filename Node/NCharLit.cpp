#include "NCharLit.hpp"

NCharLit::NCharLit() {
	name = "Char";
}

NCharLit::NCharLit(char *c) {
	this->character = *++c;
}
