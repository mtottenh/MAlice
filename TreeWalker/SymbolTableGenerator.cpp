#include "SymbolTableGenerator.hpp"
#include "../Errors/SemanticErrors.hpp"
#include "../Errors/TypeMap.hpp"
#include "../Node/TypeDefs.hpp"

SymbolTableGenerator::SymbolTableGenerator(Node* tree) {
	root = tree;
}


SymbolTable* SymbolTableGenerator::generateTable() {
	SymbolTable *table = new SymbolTable();
	root->addTable(table);
	std::deque<Node*> children = root->getChildren();
	Node* node;
	while(!children.empty()) {
		node = children.front();
		children.pop_front();
		nodeTableGen(node,table);
	}
	return table;
}

SymbolTable* SymbolTableGenerator::nodeTableGen(Node *node, SymbolTable* table) {
	int nodeType = node->getNodeType();
	Node* nodePtr = NULL;
	if(!node->addTable(table)) {
		cerr << "ERROR: Unable to  update node with symbol table pointer" 
		     << endl;
		return table;
	}
/*	cerr << "NODE TYPE: " << typemap_get(nodeType) << endl;*/
	switch(nodeType) {
		case FUNC:
		case PROCEDURE:
			nodePtr = table->lookupCurrentScope(node->getID());
			if (nodePtr == NULL) {
				table->add(node->getID(), node);
			} else {
				error_var_exists(node->getID());
				return table;
			}	
			funcGen(node,table);
			break;

		case VARDEC:
			nodePtr = table->lookupCurrentScope(node->getID());
			if (nodePtr == NULL) {
				table->add(node->getID(), node);
			} else {
				error_var_exists(node->getID());
			}	
			break;

		case CODEBLOCK:
			/* Codeblocks have their own scope so we need to 
			 * create a new scope and do the children
			 */
			table = new SymbolTable(table);
		default:
			std::deque<Node *> children = node->getChildren();
			unsigned int size = children.size();
			for (unsigned int i = 0; i < size; i++) {
				nodeTableGen(children[i],table);		
			}
			break;
	}
	return table;
}


SymbolTable* SymbolTableGenerator::funcGen(Node* func, SymbolTable* table) {
	std::deque<Node *> children = func->getChildren();
	unsigned int size = children.size();
	/*Functions with no paramaters*/
	if (size == 1) {
		nodeTableGen(children[0],table);
	} else {
		/*Deal with paramaters */
		Node* codeBlock = children[0];
		Node* paramBlock = children[1];
		SymbolTable* localScope = new SymbolTable(table);
		nodeTableGen(paramBlock,localScope);
		
		/*Deal with the function body*/
		children = codeBlock->getChildren();
		size = children.size();
		Node *decList, *statList;

		/* Deal with declarations if nessecary*/
		if (size > 1) {
			decList = children[0];
			statList = children[1];
			nodeTableGen(decList,localScope);
		} else {
			statList = children[0];
		}
			
		nodeTableGen(statList,localScope);
	}	
	return  table;	
}





























