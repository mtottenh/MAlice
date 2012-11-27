#include "Node.hpp"
#include "TypeDefs.hpp"
/* TODO Remove this #define*/
#define UNDEFINED -1
Node::Node() : loc() {
	name = "Node";
	type = INVALIDTYPE;
	nodeType = GENERIC_NODE;
	children.clear();
}

Node::Node(Node *child) {
	children.clear();
	children.push_back(child);
	name = "Node";
	type = INVALIDTYPE;
	nodeType = GENERIC_NODE;
	loc = NULL;
}
/* TODO Implement explicit deconstructor*/
/* CONVERT LOC TO SHARED PTR*/
Node::~Node() {
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		delete children[i];
		children[i] = NULL;
	}
	delete loc;
	loc = NULL;
	if (table.get() != NULL) {
		table.reset();		
	}	
}

int Node::print() const {
	cout << name << endl;
	return 1;
}
const node_children_t* Node::getChildrenRef() const {
	return &children;
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

int Node::addTable(boost::shared_ptr<SymbolTable> t) {
	if(t == NULL) {
		return 0;
	}
	else {
		this->table = t;

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

void Node::setLocation(FileLocation location) 
{
	this->loc = new FileLocation(location);
}

FileLocation* Node::getLocation()
{
	if (children.size() > 0 && children[0] != NULL && loc == NULL) {
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
void Node::addChild(Node* node) {
	children.push_back(node);
}
string Node::getName() {
	return name;
}
int Node::hasTable() {
	return table == NULL;
}
void Node::printTable() {
	table->print();
}
int Node::getChildrenSize() {
	return children.size();
}
Node* Node::getChild(unsigned int n) {
	if (n > children.size() ) {
		return NULL;
	} else {
		return children[n];
	}
}
void Node::printErrorHeader(const string& context) {
	cerr << endl << "--- Semantic error in " << context << " at line(s) " 
		<< getLocation()->startLine << "-" << getLocation()->endLine 
		<< ", column(s) " << getLocation()->startColumn << "-" 
		<< getLocation()->endColumn << " ---" << endl;
}	
