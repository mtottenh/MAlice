#include "Node/Node.hpp"
#include "TreePrinter.hpp"
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
		const Node *temp = printList.front();
		printList.pop();
		if (temp != NULL) {
			int current_level = level.front();
			level.pop();
			if(print_level< current_level) {
				cout << endl;	
				print_level = current_level;
				cout << "Level : " << print_level << endl;
			}
			cout << "Type: ";
			temp->print();
			cout << "\t"; 
			const std::vector<Node *> children = temp->getChildren();
/*			cout << "\tCurrent Node has: " << children.size() << " children" ; */
			
			for (unsigned int i = 0; i< children.size(); i++){
//				if((children[i] != temp) && (children[i] != NULL)){
					printList.push(children[i]);
					level.push(current_level + 1);
//				} else {
//					cout << "\tPROBLEM NODE AT i = " << i;
//				}
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
