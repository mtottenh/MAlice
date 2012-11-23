#include "NReturn.hpp"
NReturn::NReturn(Node* exp)
{
	name = "Return";
	children.push_back(exp);
}
