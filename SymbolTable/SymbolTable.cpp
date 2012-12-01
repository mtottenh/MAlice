#include "SymbolTable.hpp"
#include <iostream>

/* See SymbolTable.hpp for detailed documentation. */
/* Constructors and destructors. */
SymbolTable::SymbolTable() : parent(NULL) {
	/* Do nothing! */
    table_id = 1;
}

SymbolTable::SymbolTable(SymbolTable* parentTable) : parent(parentTable) {
    this->table_id = parentTable->getTableID() + 1;
}

/* Public methods. */
int SymbolTable::add(string name, Node* node) {
	Node* currentNodePtr = lookupCurrentScope(name);
	
	if (currentNodePtr == NULL) {
		table[name] = node;
		return 1;
	}
	else {
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
	cout << endl << "----- TABLE ID " << table_id << " -----" << endl;
 	table_t::iterator it;
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


void SymbolTable::setTableID(int id) {
    this->table_id = id;
}

int SymbolTable::getTableID() {
    return table_id;
}
