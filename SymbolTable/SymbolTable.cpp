#include "SymbolTable.hpp"
#include <iostream>



/* See SymbolTable.hpp for detailed documentation. */
/* print function faily self explanitory*/
void SymbolTable::print() {
	cout << "\n\n**** SYMBOL TABLE CONTENT ****\n\n";
 	table_t::iterator it;
//	it = table.begin();
	for(it = table.begin(); it != table.end(); ++it) {
		cout << "Name: " << it->first;
		cout << endl;
	}
	cout << "\n**** END SYMBOL TABLE CONTENT ****\n";
}
/* Constructors and destructors. */
SymbolTable::SymbolTable() : parent(NULL) {
	/* Do nothing! */
}

SymbolTable::SymbolTable(SymbolTable* parentTable) : parent(parentTable) {
	/* Do nothing! */
}

SymbolTable::~SymbolTable() {
	table_t::iterator it;

	for (it = table.begin(); it != table.end(); ++it) {
		delete it->second;
	}
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

	if (currentNodePtr != NULL || table.size() == 0) {
		return currentNodePtr;
	}
	else if (parentExists()) {
		return parent->lookup(name);
	}
	else {
		return NULL;
	}
}

/* Private methods. */
Node* SymbolTable::lookupCurrentScope(const string& name) {
	table_t::iterator nameNodePtr = table.find(name);

	if (nameNodePtr == table.end() || table.size() == 0) {
		return NULL;
	}
	else {
		return nameNodePtr->second;
	}
}

int SymbolTable::parentExists() {
	return !(parent == NULL);
}
