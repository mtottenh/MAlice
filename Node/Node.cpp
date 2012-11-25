#include "Node.hpp"
#include "TypeDefs.hpp"

#define UNDEFINED -1

Node::Node() {
	name = "Node";
	type = INVALIDTYPE;
}

Node::Node(Node *child) {
	children.push_back(child);
	name = "Node";
	type = INVALIDTYPE;
}

Node::~Node() {
	
}

int Node::print() const {
	cout << name << endl;
	return 1;
}

node_children_t Node::getChildren() const {
	return children;
}

int Node::getType()  {
	return resolveType();
}

int Node::resolveType() {
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

int Node::getNodeType() {
	return nodeType;
}

int Node::isRoot() {
	isRootNode = 1;
	return isRootNode;
}

int Node::compareTypes(int t1, int t2) const {
	return (t1 == t2);
/*	switch(t1) {
		case TNUMBER:
		case REFNUMBER:
			return (t2 == TNUMBER || t2 == REFNUMBER);
			break;
		case TCHAR:
		case REFCHAR:
			return (t2 == TCHAR || t2 == REFCHAR);
			break;
		case TSTRING:
		case REFSTRING:
			return (t2 == TSTRING || t2 == REFSTRING);
			break;
		default:
			return (t1 == t2);
			break;
	} */		
}	
