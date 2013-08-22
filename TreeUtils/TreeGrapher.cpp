#include "../Errors/TypeMap.hpp"
#include "TreeGrapher.hpp"

TreeGrapher::TreeGrapher() {
 
}
 
void TreeGrapher::visit(NArrayAccess* node)  {
     conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }
        

}

void TreeGrapher::visit(NAssignment* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NBinOp* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NCharLit* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NCodeBlock* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NConditional* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NDec* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NDeclarationBlock* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }
}

void TreeGrapher::visit(NEndIf* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NFunctionDeclaration* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NIdentifier* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NInc* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NInput* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NInteger* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}

void TreeGrapher::visit(NLoop* node) {

}

void TreeGrapher::visit(NMethodCall* node) {

}

void TreeGrapher::visit(NNullToken* node) {

}

void TreeGrapher::visit(Node* node) {
    
}

void TreeGrapher::visit(NParamBlock* node) {
}

void TreeGrapher::visit(NParamDeclarationBlock* node) {

}

void TreeGrapher::visit(NPrint* node) {

}

void TreeGrapher::visit(NReturn* node) {

}

void TreeGrapher::visit(NStatementList* node) {

}

void TreeGrapher::visit(NStringLit* node) {

}

void TreeGrapher::visit(NUnaryOp* node) {

}

void TreeGrapher::visit(NVariableDeclaration* node) {
    conditional_add(node);
   /* next create its children and the edges between them 
     * and accept each one
     */
    for (int i = 0; i < node->getChildrenSize(); i++) {
        Node* child = node->getChild(i);
        conditional_add(child);
        boost::add_edge(node->getGraphNode(),child->getGraphNode(),this->g);
        node->getChild(i)->accept(this);
    }

}


void TreeGrapher::outputGraph() {
    ofstream outf("graph.gv");
    write_graphviz(std::cout, this->g, boost::make_label_writer(boost::get(&Vertex::node_name, this->g)));
}

void TreeGrapher::conditional_add(Node* node) {
    /* If the node hasn't been added to the graph then add it */
   if (!node->graphed()) {
       node->setGraphNode(boost::add_vertex(g));
       g[node->getGraphNode()].node_name = node->getID();
       g[node->getGraphNode()].node_type = typemap_get(node->getNodeType());
   }
 
}
