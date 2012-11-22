#include "NInteger.hpp"

NInteger::NInteger() {
	name = "Integer";
}

NInteger::NInteger(int value) {
	this->value = value;
	name = boost::lexical_cast<string>(value);
}
