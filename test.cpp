#include "node.h"
#include <queue>
int tree_print(Node *root) {
	std::queue<Node *> printList;
	printList.push(root);
	while(printList.empty()) {
		Node *temp = printList.pop();
		temp->print();
		printlist.push(temp->children[1..n]);
	}
	
}
