#include "Node.hpp"
#include "TypeDefs.hpp"
/* TODO Remove this #define*/
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
/* TODO Implement explicit deconstructor*/
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
/* TODO rename to setAsRoot()
 */
int Node::isRoot() {
	isRootNode = 1;
	return isRootNode;
}
/* TODO Really? do we need this as a function?
 */
int Node::compareTypes(int t1, int t2) const {
	return (t1 == t2);
}

void Node::setLocation(fileLocation location) 
{
	this->location = location;
}
fileLocation Node::getLocation()
{
	/*location.startLine == 0 iff location has not been initialised.*/
	if (location.startLine == 0)
	{
		location.startLine = children.front()->getLocation().startLine;
		location.startColumn = children.front()->getLocation().startColumn;
		location.endLine = children.back()->getLocation().endLine;
		location.endColumn = children.back()->getLocation().endColumn;
	}
	return location;
}	
