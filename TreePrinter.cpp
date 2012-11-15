#include "node.hpp"
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
		temp->print(); 
		const std::vector<Node *> children = temp->getChildren();
		for (int i = 0; i< children.size(); i++){
			printList.push(children[i]);
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
