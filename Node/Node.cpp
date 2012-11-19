#include "Node.hpp"

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
