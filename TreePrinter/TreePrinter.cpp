#include "TreePrinter.hpp"
#include "../Errors/TypeMap.hpp"

#include <queue>
#include <iostream>
using namespace std;
treePrinter::treePrinter(Node *tree) { 
	root = tree; 
}
int treePrinter::print() {
	std::queue<const Node *> printList;
	std::queue<int> level;
	int print_level = 1;
	level.push(print_level);
	printList.push(root);
	while(!printList.empty()) {
		cout << endl;
		const Node *temp = printList.front();
		printList.pop();
		int current_level = level.front();
		level.pop();
		if(print_level< current_level) {
			cout << endl;	
			print_level = current_level;
			cout << ">>>>> Printing Level " << print_level <<
				" <<<<<" << endl;
		}
		if (temp != NULL) {
			cout << "~~~~~ New node! ~~~~~" << endl;	
			cout << "Type: " << typemap_get(temp->getType()) 
				<< endl;
			if (temp->table != NULL)
				temp->table->print();
			else
				cout << "Null symbol table detected!" << endl;
		
		node_children_t children = temp->getChildren();
/*		cout << "\tCurrent Node has: " << children.size() << " children" ; */
		 				
			for (unsigned int i = 0; i< children.size(); i++){
			//	if((children[i] != temp) && (children[i] != NULL)){
					printList.push(children[i]);
					level.push(current_level + 1);
			//	} else {
			//		cout << "\tPROBLEM NODE AT i = " << i;
			//	}
			}
		}
	}
	cout << endl;
	return 0;
}

/* used for some basic testing 
int main () {
	cout << "START AST PRINTER TEST" << endl;;
	treePrinter t(new NExpression);
	t.print();
	cout << endl << "END AST PRINTER TEST" << endl;
	return 0;
} */
