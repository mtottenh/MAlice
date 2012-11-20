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

/* return the type as or UNDEFINED */
int Node::getType() const {
	return UNDEFINED;
}
