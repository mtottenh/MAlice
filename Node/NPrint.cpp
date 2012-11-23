#include "NPrint.hpp"

NPrint::NPrint() {
	name = "Print";
}

NPrint::NPrint(const char* msg) {
	message = strdup(msg); 
	name = "Print";
}

NPrint::NPrint(Node* child) { 
	name = "print";
	children.push_back(child); 
}
