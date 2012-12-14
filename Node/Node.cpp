#include "Node.hpp"
#include "TypeDefs.hpp"
#include "../CodeGeneration/ASTVisitor.hpp"
/* Constructors/destructors. */

/*
 * Construct a node with no children; set to unknown data type and set as a 
 * generic node.
 */
Node::Node() : loc(), label("") {
	name = "Node";
	type = INVALIDTYPE;
	nodeType = GENERIC_NODE;
	children.clear();
}

/* As above, but add the child parameter to the list of children. */
Node::Node(Node *child) {
	children.clear();
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
	delete loc;
	loc = NULL;
	if (table.get() != NULL) {
		table.reset();		
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


FileLocation* Node::getLocation() {
	/* Get the location from the first and last child in children. */
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

int Node::setRoot() {
	isRootNode = 1;
	return isRootNode;
}
int Node::isRoot() {
    return isRootNode;
}
int Node::hasTable() {
	return table == NULL;
}

void Node::setLocation(FileLocation loc) {
	/* Create a new FileLocation given an existing location. */
	this->loc = new FileLocation(loc);
}
void Node::setLocation(FileLocation* loc) {
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

int Node::addTable(boost::shared_ptr<SymbolTable> t) {
	if(t.get() == NULL) {
		return FAILURE;
	}
	else {
		this->table = t;
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
		isValid &= (*it)->check();
	}

	return isValid;
}

/* TODO Remove this, and replace all occurances in derived classes. */
int Node::compareTypes(int t1, int t2) const {
	return (t1 == t2);
}

void Node::accept(ASTVisitor* v) {
    cerr << "Generic Node" << endl;
    v->visit(this);
}

int Node::getSize() {
    switch(this->getType()) {
        case TNUMBER:
            return 8;
        case PROCEDURE:
        case  FUNC:
            return 90001;
    } 
}

int Node::getWeight() {
	if(weight == -1 ) {
		return calculateWeight();
	}

	return weight;
}
int Node::getLevel() {
	if (table == NULL) {
		return -1;
	}
	return table->getTableID();
}



/* Protected methods. */

int Node::resolveType() {
	return type;
}	


boost::shared_ptr<SymbolTable> Node::getTable() {
    return table;
}

int Node::calculateWeight() {
	/* 
	 * Base implementation - the weight of the node is the sum of the weights of
	 * its children.
	 */
	for(int i = 0; i < this->getChildrenSize(); ++i) {
		weight += this->getChild(i)->getWeight();
	}

	return weight;
}

string Node::getLabel() {
	return this->label;
}

void Node::setLabel(string label) {
	this->label = label;
}
