#include "NInteger.hpp"
#include "TypeDefs.hpp"

NInteger::NInteger() {
	name = "Integer";
	nodeType = INTEGER;
}

NInteger::NInteger(int value) {
	this->value = value;
	this->type = resolveType();
	name = boost::lexical_cast<string>(value);
	nodeType = INTEGER;
}

int NInteger::resolveType() {
	return TNUMBER;
}

int NInteger::check() {
	return 1;
}
