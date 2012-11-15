#include "node.hpp"
class treePrinter {
private:
	const Node *root;
public:
	treePrinter(Node *tree);  
	int print();
};
