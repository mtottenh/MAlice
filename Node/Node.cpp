#include "Node.hpp"
#include "TypeDefs.hpp"

/* Constructors/destructors. */

/*
 * Construct a node with no children; set to unknown data type and set as a 
 * generic node.
 */
Node::Node() {
	name = "Node";
	type = INVALIDTYPE;
	nodeType = GENERIC_NODE;
	loc = NULL;
}

/* As above, but add the child parameter to the list of children. */
Node::Node(Node *child) {
	children.push_back(child);
	name = "Node";
	type = INVALIDTYPE;
	nodeType = GENERIC_NODE;
	loc = NULL;
}

/* Delete all the children and set them to null. */
Node::~Node() {
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		delete children[i];
		children[i] = NULL;
	}
	if (loc != NULL) {
		delete loc;
		loc = NULL;
	}
	if (table != NULL) {
		if (markedForDeletion) {
			markedForDeletion = 0;
		} else {
			delete table;
			table = NULL;
			markedForDeletion = 1;
		}
	}
}

/* Public methods. */

int Node::getType()  {
	/* Resolve the type before we return it. */
	return resolveType();
}

int Node::getNodeType() {
	return nodeType;
}

string Node::getID() {
	return name;
}

node_children_t Node::getChildren() const {
	return children;
}

const node_children_t* Node::getChildrenRef() const {
	return &children;
}

Node* Node::getChild(unsigned int n) {
	if (n > children.size() ) {
		return NULL;
	} else {
		return children[n];
	}
}

int Node::getChildrenSize() {
	return children.size();
}

void Node::setLocation(FileLocation *location) {
	this->loc = new FileLocation(location);
	//delete(location);
}

FileLocation* Node::getLocation() {
	/* Get the location from the first and last child in children. */
	if (children.size() > 0) {
		loc = new FileLocation();
		Node *front = children.front();
		Node *back = children.back();	

		if (front->getLocation() != NULL ) {
			loc->startLine = front->getLocation()->startLine;
			loc->startColumn = front->getLocation()->startColumn;
		} 

		if (back->getLocation() != NULL ) {		
			loc->endLine = back->getLocation()->endLine;
			loc->endColumn = back->getLocation()->endColumn;
		}
	}

	return loc;
}

/* TODO rename to setAsRoot()
 */
int Node::isRoot() {
	isRootNode = 1;
	return isRootNode;
}

int Node::hasTable() {
	return table == NULL;
}

void Node::setLocation(FileLocation *loc) {
	/* Create a new FileLocation given an existing location. */
	this->loc = new FileLocation(loc);
}

int Node::print() const {
	/* Print the node name to stdout. */
	cout << name << endl;
	return SUCCESS;
}

void Node::printTable() {
	table->print();
}

void Node::printErrorHeader(const string& context) {
	/* Print an error header containing the context and line/col numbers. */
	cerr << endl << "--- Semantic error in " << context << " at line(s) " 
		<< getLocation()->startLine << "-" << getLocation()->endLine 
		<< ", column(s) " << getLocation()->startColumn << "-" 
		<< getLocation()->endColumn << " ---" << endl;
}

int Node::addTable(SymbolTable* table) {
	if(table == NULL) {
		return FAILURE;
	}
	else {
		this->table = table;
		return SUCCESS;
	}
}

void Node::addChild(Node* node) {
	children.push_back(node);
}

int Node::check() {
	/* Generic check function - loop through the children and check them. */
	int isValid = 1;
	node_children_t::iterator it;

	for(it = children.begin(); it != children.end(); ++it) {
		isValid = (*it)->check();
	}

	return isValid;
}

/* TODO Remove this, and replace all occurances in derived classes. */
int Node::compareTypes(int t1, int t2) const {
	return (t1 == t2);
}

/* Protected methods. */

int Node::resolveType() {
	return type;
}	
