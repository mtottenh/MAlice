#include "Node.hpp"

Node::Node() {
	name = "Node";
}

Node::Node(Node *a) {
	children.push_back(a);
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
