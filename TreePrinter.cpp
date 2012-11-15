#include "Node.hpp"
#include "TreePrinter.hpp"
#include <queue>
#include <iostream>
using namespace std;
treePrinter::treePrinter(Node *tree) { 
	root = tree; 
}
int treePrinter::print() {
	std::queue<const Node *> printList;
	printList.push(root);
	while(!printList.empty()) {
		const Node *temp = printList.front();
		printList.pop();
		if (temp != NULL) {
			temp->print(); 
			const std::vector<Node *> children = temp->getChildren();
			cout << "Current Node has: " << children.size() << " children" << endl;
			for (int i = 0; i< children.size(); i++){
				if(children[i] != temp) {
					printList.push(children[i]);
				}
		}
	
}
	}
};

/* used for some basic testing 
int main () {
	cout << "START AST PRINTER TEST" << endl;;
	treePrinter t(new Node);
	t.print();
	cout << endl << "END AST PRINTER TEST" << endl;
	return 0;
} */
