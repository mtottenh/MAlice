#include "NInteger.hpp"
#include "TypeDefs.hpp"

/* Constructors. */
NInteger::NInteger() {
	this->name = "Integer";
	this->nodeType = INTEGER;
}

NInteger::NInteger(int value) {
	this->value = value;
	this->type = resolveType();

	/* Cast the int to a string to get a representation of its value. */
	this->name = boost::lexical_cast<string>(value);

	this->nodeType = INTEGER;
}

/* Public methods. */

int NInteger::check() {
	/* As a primitive base type, an integer node is always valid. */
	return 1;
}

/* Protected methods. */

int NInteger::resolveType() {
	/* An integer is always a number! */
	return TNUMBER;
}
