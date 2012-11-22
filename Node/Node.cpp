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

node_children_t Node::getChildren() const {
	return children;
}

int Node::getType() const {
	return type;
}

string Node::getID() {
	return name;
}

int Node::check() {
	int isValid = 1;
	node_children_t::iterator it;

	for(it = children.begin(); it != children.end(); ++it) {
		isValid = (*it)->check();
	}

	return isValid;
}

int Node::addTable(SymbolTable* table) {
	if(table == NULL) {
		return 0;
	}
	else {
		this->table = table;
		return 1;
	}
}
