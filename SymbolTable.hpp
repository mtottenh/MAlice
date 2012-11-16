#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <boost/functional/hash.hpp>  
#include "Node.hpp" /* change to Node.hpp */

/*
 * Symbol table for variables and identifiers contained within a MAlice program.
 * Supports adding mappings between variables/identifiers and their values and
 * retrieval of values based on their variable/identifier name.
 *
 * A symbol table will have a pointer to its parent symbol table (to look up a
 * variable in parent scope), unless it is the global variable table which will
 * instead have its parent set to NULL. The variable/identifer to value mapping
 * are stored in a hash map, making use of the Boost library's hash function.
 */

class SymbolTable {
	private:
		/*
		 * Fields representing the hash map of identifers/variables to
		 * values, a string hash function and the pointer to the parent
		 * symbol table.
		 */
		map<int, Node*> dictionary;
		boost::hash<string> string_hash;	
		SymbolTable* parent;
		
		/* Look up a given variable/identifier in this table. */
		Node lookupCurrentNode(const string&);
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
		 * Returns 0 if successful, -1 on failure.
		 */
		int add(string, Node&);

		/*
		 * Look up a given variable/identifier in this table and all
		 * parent tables. Returns the Node if found, NULL otherwise.
		 */
		Node lookup(const string&);
};

#endif
