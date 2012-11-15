#include "node.h"
#include <queue>


int tree_print(Node *root) {
	std::queue<Node *> printList;
	printList.push(root);
	while(printList.empty()) {
		Node *temp = printList.pop();
		temp->print();
		std::queue<Node *> children = temp->getchildren();
		while (children.empty()) {
			printlist.push(children.pop());
		}
	}
	
}
