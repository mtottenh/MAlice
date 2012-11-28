#include <boost/shared_ptr.hpp>
#include "SymbolTableGenerator.hpp"
#include "../Errors/SemanticErrors.hpp"
#include "../Errors/TypeMap.hpp"
#include "../Node/TypeDefs.hpp"

SymbolTableGenerator::SymbolTableGenerator(Node* tree) {
	root = tree;
}


int SymbolTableGenerator::generateTable() {
    int isValid = 1;
	sym_table_ptr table (new SymbolTable());
	root->addTable(table);
	std::deque<Node*> children = root->getChildren();
	Node* node;
	while(!children.empty()) {
		node = children.front();
		children.pop_front();
		isValid &= nodeTableGen(node,table);
	}
    return isValid;
}

int SymbolTableGenerator::nodeTableGen(Node *node, sym_table_ptr table) {
    int isValid = 1;
	int nodeType = node->getNodeType();
	Node* nodePtr = NULL;
	std::deque<Node *> children = node->getChildren();
	unsigned int size = children.size();

	if(!node->addTable(table)) {
		cerr << "ERROR: Unable to  update node with symbol table pointer" 
		     << endl;
        isValid = 0;
		return isValid;
	}
	
	switch(nodeType) {
		case FUNC:
		case PROCEDURE:
			nodePtr = table->lookupCurrentScope(node->getID());
			if (nodePtr == NULL) {
				table->add(node->getID(), node);
			} else {
				error_var_exists(node->getID());
                isValid = 0;
				return isValid;
			}	
			isValid &=funcGen(node,table);
			break;

		case VARDEC:
			nodePtr = table->lookupCurrentScope(node->getID());
			if (nodePtr == NULL) {
				table->add(node->getID(), node);
			} else {
				error_var_exists(node->getID());
                isValid = 0;
			}	
			for (unsigned int i = 0; i < size; i++) {
				isValid &= nodeTableGen(children[i],table);		
			}

			break;

		case CODEBLOCK:
			/* Codeblocks have their own scope so we need to 
			 * create a new scope and do the children
			 */
			table = boost::shared_ptr<SymbolTable> (new SymbolTable(table.get()));
		default:
			for (unsigned int i = 0; i < size; i++) {
				isValid &= nodeTableGen(children[i],table);		
			}
			break;
	}
	return isValid;
}


int SymbolTableGenerator::funcGen(Node* func, sym_table_ptr table) {
    int isValid = 1;
	std::deque<Node *> children = func->getChildren();
	unsigned int size = children.size();
	/*Functions with no paramaters*/
	if (size == 1) {
		isValid &= nodeTableGen(children[0],table);
	} else {
		/*Deal with paramaters */
		Node* codeBlock = children[0];
		Node* paramBlock = children[1];
		sym_table_ptr localScope (new SymbolTable(table.get()));
		isValid &= nodeTableGen(paramBlock,localScope);
		
		/*Deal with the function body*/
		children = codeBlock->getChildren();
		codeBlock->addTable(localScope);
		size = children.size();
		Node *decList, *statList;

		/* Deal with declarations if nessecary*/
		if (size > 1) {
			decList = children[0];
			statList = children[1];
			isValid &= nodeTableGen(decList,localScope);
		} else {
			statList = children[0];
		}
			
		isValid &= nodeTableGen(statList,localScope);
	}

	return  isValid;	
}





























