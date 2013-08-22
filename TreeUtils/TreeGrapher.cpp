#include "../Errors/TypeMap.hpp"
#include "TreeGrapher.hpp"

TreeGrapher::TreeGrapher() {
     outf.open("graph.gv");
}
 
TreeGrapher::TreeGrapher(string fName) {
    outf.open(fName.c_str());
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

void TreeGrapher::visit(NMethodCall* node) {
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

void TreeGrapher::visit(NNullToken* node) {
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

void TreeGrapher::visit(Node* node) {
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

void TreeGrapher::visit(NParamBlock* node) {
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

void TreeGrapher::visit(NParamDeclarationBlock* node) {
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

void TreeGrapher::visit(NPrint* node) {
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

void TreeGrapher::visit(NReturn* node) {
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

void TreeGrapher::visit(NStatementList* node) {
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

void TreeGrapher::visit(NStringLit* node) {
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

void TreeGrapher::visit(NUnaryOp* node) {
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

    write_graphviz(this->outf, this->g, boost::make_label_writer(boost::get(&Vertex::node_name, this->g)));
}

void TreeGrapher::conditional_add(Node* node) {
    /* If the node hasn't been added to the graph then add it */
   if (!node->graphed()) {
       node->setGraphNode(boost::add_vertex(g));
       g[node->getGraphNode()].node_name = node->getID();
       g[node->getGraphNode()].node_type = typemap_get(node->getNodeType());
   }
 
}
