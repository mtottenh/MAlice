#include "NDec.hpp"

NDec::NDec(Node* child)
{
	name = child->name;
	children.push_back(child);
}
