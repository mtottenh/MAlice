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
#include "../Errors/SemanticErrors.hpp"
#include "../SymbolTable/SymbolTable.hpp"

#include "FileLocation.hpp"

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

typedef deque<Node *> node_children_t;


class Node {
private:
	/*
	 * FileLocation class contains details about line number, column number
	 * etc
	 */
	FileLocation *loc;
protected:
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

	/* Returns SUCCESS if the node is the root node, FAILURE otherwise. */
	int isRoot();

	/*
	 * Checks whether the node has an associated symbol table. Returns
	 * SUCCESS if one exists, FAILURE otherwise.
	 */
	int hasTable();

	/* Sets the location class of the node to a given FileLocation. */
	void setLocation(FileLocation*);

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
};

#endif
