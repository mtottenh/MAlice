#include "Node.hpp"
#define UNDEFINED -1
Node::Node() {
	name = "Node";
}

Node::Node(Node *child) {
	children.push_back(child);
	name = "Node";
}

Node::~Node() {
	
}

int Node::print() const {
	cout << name;
	return 1;
}

vector<Node*> Node::getChildren() const {
	return children;
}

int Node::getType() const {
	return type;
}

int Node::check(SymbolTable* table) {
	int isValid = 1;
	vector<Node*>::iterator it;

	for(it = children.begin(); it != children.end(); ++it) {
		isValid = it->check();
	}

	return isValid;
}
