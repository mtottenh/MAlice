#include "NChar.hpp"

NChar::NChar() {
	name = "Char";
}

NChar::NChar(char *c) {
	this->character = *c;
}
