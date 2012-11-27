#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <map>  
#include "../Node/Node.hpp"
class Node;
using namespace std;

/*
 * Typedef for symbol table internal identifier -> Node mapping. Implemented
 * as an unordered (hash) map from the Boost libraries. 
 */
typedef map<string, Node*> table_t;

/*
 * Symbol table for variables and identifiers contained within a MAlice program.
 * Supports adding mappings between variables/identifiers and their values and
 * retrieval of values based on their variable/identifier name.
 *
 * A symbol table will have a pointer to its parent symbol table (to look up a
 * variable in parent scope), unless it is the global variable table which will
 * instead have its parent set to NULL. The variable/identifer to value mapping
 * are stored in a map of type table_t.
 */

class SymbolTable {
	private:
		/*
		 * Fields representing the hash map of identifers/variables to
		 * values, a string hash function and the pointer to the parent
		 * symbol table.
		 */
		table_t table;
		SymbolTable* parent;
				
	public:
		/* 
		 * Constructors for creating a global variable table
		 * (no parameters) or a table with a parent. Common destructor.
		 */
		SymbolTable();
		SymbolTable(SymbolTable*);
		~SymbolTable();		
		/* 
		 * Add a new variable/identifier and node pairing to the table.
		 * Returns 1 if successful, 0 on failure (elem already exists
		 * in the map).
		 */
		int add(string, Node*);

		/* Look up a given variable/identifier in this table. */
		Node* lookupCurrentScope(const string&);

		/*
		 * Look up a given variable/identifier in this table and all
		 * parent tables. Returns the Node if found, NULL otherwise.
		 */
		Node* lookup(const string&);

		/*
		 * Returns whether a parent symbol table exists. 1 if it exists,
		 * 0 otherwise.
		 */
		int parentExists();
		/*nt markedForDeletion();*/
		void print();
};

#endif
