/*
 * Node base class - contains core functions for a Node within the AST.
 * Each node within the AST will implement a specialisation of this class.
 */

#ifndef _NODE_HPP_
#define _NODE_HPP_
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <deque>
#include <string>
#include <algorithm>
#include "../Errors/SemanticErrors.hpp"
#include "../SymbolTable/SymbolTable.hpp"

#include "../FileLocation/FileLocation.hpp"
/*
 * Defines for method returns - it is (apparently) bad practice to use
 * EXIT_SUCCESS and EXIT_FAILURE from stdlib.
 */
#define SUCCESS 1
#define FAILURE 0

using namespace std;

/*
 * Typedef to define the strucutre that will contain the Node's children.
 * Forward declaration of Node required since the typedef references Node.
 */
class Node;
class SymbolTable;
class ASTVisitor;
typedef deque<Node *> node_children_t;


class Node {
protected:
	/*
	 * FileLocation class contains details about line number, column number
	 * etc
	 */
	FileLocation *loc;

	/* Symbol table maps identifiers to Nodes that this Node references. */
	boost::shared_ptr<SymbolTable> table;

	/* Node type states the type of the node, e.g. function node. */
	int nodeType;

	/* Type refers to the data type of the node, e.g. letter. */
	int type;

	/* Flag that determines whether this node is the root of the AST. */
	int isRootNode;

	/* Data structure containing the node's children. */
	node_children_t children; 

	/* String representing the name of the node - usually its identifier. */
	string name;
	
	/* String representing the label of a declared variable */
	string label;

	/* Field for the weight of the node (number of registers required). */
	int weight; 
    
    /* reference count used for code optimisation */
    int refCount;
    /* used by the TreeGrapher visitor to determin if a node has been graphed */
    bool is_graphed = false;
	/* Calculate the weight of the node. */
	virtual int calculateWeight();

	/* 
	 * Virtual method that determines the type of the node. Returns an 
	 * integer corresponding to the node's type.
	 */
	virtual int resolveType();
public:	
	Node();
	Node(Node*);
	virtual ~Node();

	/* Returns an integer representing the data type of the node. */
	virtual int getType();

	/* Returns an integer representing the node type, e.g. function. */
	virtual int getNodeType();

	/* Returns a string representing the name/identifier of the node. */
	virtual string getID();

	/*
	 * Returns a structure of type node_children_t containing the node's
	 * children.
	 */
	node_children_t getChildren() const;

	/*
	 * Returns a reference to a structure of type node_children_t containing
	 * the node's children.
	 */
 	const node_children_t* getChildrenRef() const;

	/*
	 * Returns a pointer to the node at a given index in the children list.
	 * Returns NULL if the given index is out of bounds.
	 */
	Node* getChild(unsigned int);

	/* Returns the size of the list of this node's children. */
	int getChildrenSize();

	/* Returns a FileLocation pointer giving the node's location. */
	FileLocation* getLocation();

	/* Returns the weight of the node. */
	int getWeight();

	/* Returns SUCCESS if the node is the root node, FAILURE otherwise. */
	int isRoot();
    /* sets the node to be the root node */
    int setRoot();
    /* remove child */
    void removeChild(int);
	/*
	 * Checks whether the node has an associated symbol table. Returns
	 * SUCCESS if one exists, FAILURE otherwise.
	 */
	int hasTable();

	/* Sets the location class of the node to a given FileLocation. */
	void setLocation(FileLocation);
	void setLocation(FileLocation *);

	/* Sets the label in assembly code of a declaration for reference */
	void setLabel(string);

	/* Get the label associated with the declared variable */
	string getLabel();

	/* Prints the contents of the node. Returns SUCCESS or FAILURE. */
	virtual int print() const;

	/* Prints the node's symbol table, if one exists. */
	void printTable();

	/* 
	 * Prints an error header, containing the error context (given by the
	 * string parameter) and line/column numbers from the node's
	 * FileLocation class.
	 */
	virtual void printErrorHeader(const string&);

	/*
	 * Adds a pointer to a symbol table to the node. Returns SUCCESS if the
	 * operation completed successfully, FAILURE otherwise.
	 */
	virtual int addTable(boost::shared_ptr<SymbolTable>);

	/*
	 * Adds a node given by the Node* parameter to the list of this node's
	 * children.
	 */
	void addChild(Node*);

	/*
	 * Check function that ensures the node and it's children are valid
	 * given the semantics of the MAlice language. Returns SUCCESS if the
	 * node is semantically correct, FAILURE otherwise.
	 * This base implementation of semantic checking will simply call the
	 * check function on each child of the node.
	 */
	virtual int check();

	/* TODO Remove this function. */
	virtual int compareTypes(int, int) const;

    /* Accept function used for code generation*/
    virtual void accept(ASTVisitor*);
    /* get symbol table function*/
    boost::shared_ptr<SymbolTable> getTable();
    int getSize();
	/* returns the 'nesting level' of the node */
	int getLevel();
    /* getters and setters for the reference count */
    int getRefCount();
    void incRefCount();
};

#endif
