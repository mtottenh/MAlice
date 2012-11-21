#include "NBinOp.hpp"

NBinOp::NBinOp(Node* left, Node* right, BinaryOperator op) : binOp(op) {
	name = "Binary Operator";
	children.push_back(right);
	children.push_back(left);
}
