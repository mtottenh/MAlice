#include "NStringLit.hpp"
#include <string.h>

NStringLit::NStringLit(char* literal)
{
	//Remove the preceeding and following single quote
	this->literal = strdup(literal);
	this->literal = this->literal.substr(1, this->literal.size() - 1);
}
