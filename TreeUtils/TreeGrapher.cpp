#include "TreeGrapher.hpp"
#include <boost/graph/adjacency_list.hpp>

TreeGrapher::TreeGrapher() {
 
}
 
void TreeGrapher::visit(NArrayAccess* node)  {
         

}

void TreeGrapher::visit(NAssignment* node) {

}

void TreeGrapher::visit(NBinOp* node) {

}

void TreeGrapher::visit(NCharLit* node) {

}

void TreeGrapher::visit(NCodeBlock* node) {

}

void TreeGrapher::visit(NConditional* node) {

}

void TreeGrapher::visit(NDec* node) {

}

void TreeGrapher::visit(NDeclarationBlock* node) {
    std::cout << "Visited node" << std::endl;
    
    vertex_t current_node = boost::add_vertex(this->g);

}

void TreeGrapher::visit(NEndIf* node) {

}

void TreeGrapher::visit(NFunctionDeclaration* node) {

}

void TreeGrapher::visit(NIdentifier* node) {

}

void TreeGrapher::visit(NInc* node) {

}

void TreeGrapher::visit(NInput* node) {

}

void TreeGrapher::visit(NInteger* node) {

}

void TreeGrapher::visit(NLoop* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NMethodCall* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NNullToken* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(Node* node) {
    
    std::cout << "Visited node" << std::endl;

}

void TreeGrapher::visit(NParamBlock* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NParamDeclarationBlock* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NPrint* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NReturn* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NStatementList* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NStringLit* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NUnaryOp* node) {
    std::cout << "Visited node" << std::endl;
}

void TreeGrapher::visit(NVariableDeclaration* node) {
    std::cout << "Visited node" << std::endl;
}

