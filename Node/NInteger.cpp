#include "NInteger.hpp"
#include "TypeDefs.hpp"

NInteger::NInteger() {
	name = "Integer";
}

NInteger::NInteger(int value) {
	this->value = value;
	this->type = resolveType();
	name = boost::lexical_cast<string>(value);
}

int NInteger::resolveType() {
	return TNUMBER;
}

int NInteger::check() {
	return 1;
}
