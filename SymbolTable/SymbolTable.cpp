#include "SymbolTable.hpp"
#include <iostream>

/* See SymbolTable.hpp for detailed documentation. */

/* Constructors and destructors. */
SymbolTable::SymbolTable() : parent(NULL) {
	/* Do nothing! */
}

SymbolTable::SymbolTable(SymbolTable* parentTable) : parent(parentTable) {
	/* Do nothing! */
}

SymbolTable::~SymbolTable()
{
	cout << "deleting" << this <<  endl;
}

/* Public methods. */
int SymbolTable::add(string name, Node* node) {
	Node* currentNodePtr = lookupCurrentScope(name);
	
	if (currentNodePtr == NULL) {
		table[name] = node;
		/* debug code */
		string tname = table.find(name)->first;
		cout << "SYMTABLE: Adding " << tname << endl;
		/* end debug code */
		return 1;
	}
	else {
		cout << "SYMTABLE: Did NOT add " << name << endl;
		return 0;
	}
}

Node* SymbolTable::lookup(const string& name) {
	Node* currentNodePtr = lookupCurrentScope(name);

	if (currentNodePtr != NULL ) {
		return currentNodePtr;
	}
	else if (parentExists()) {
		return parent->lookup(name);
	}
	else {
		return NULL;
	 }
}

void SymbolTable::print() {
	cout << endl << "----- Printing Symbol Table -----" << endl;
 	table_t::iterator it;
//	it = table.begin();
	for(it = table.begin(); it != table.end(); ++it) {
		cout << "Identifier: " << it->first;
		cout << "\t\t\tNode name: " << it->second->getID() << endl;
	}
	cout << endl << "----- End Symbol Table -----" << endl;
}

/* Private methods. */
Node* SymbolTable::lookupCurrentScope(const string& name) {
	if (table.empty()) {
		return NULL;
	}
	table_t::iterator nameNodePtr = table.find(name);
	if (nameNodePtr == table.end()) {
		return NULL;
	}
	else {
		return nameNodePtr->second;
	}
}

int SymbolTable::parentExists() {
	return !(parent == NULL);
}
