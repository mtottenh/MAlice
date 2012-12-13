#include "TreeOptimiser.hpp"
#include "../Node/NodeIncludes.hpp"
TreeOptimiser::TreeOptimiser() {

}

void TreeOptimiser::visit(NArrayAccess *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}

void TreeOptimiser::visit(NAssignment *node){

}

void TreeOptimiser::visit(NBinOp *node){

}

void TreeOptimiser::visit(NCharLit *node){

}

void TreeOptimiser::visit(NCodeBlock *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}

void TreeOptimiser::visit(NConditional *node){

}

void TreeOptimiser::visit(NDec *node){

}

void TreeOptimiser::visit(NDeclarationBlock *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
        switch(node->getChild(i)->getNodeType()) {
            case FUNC:
            case PROCEDURE:
                cerr << "Testing function for removal candidate\n";
                if (node->getChild(i)->getRefCount() == 0 && node->getChild(i)->getID() != "hatta") {
                    cerr << "Function: "<< node->getChild(i)->getID() << " removed from AST\n";   
                    node->removeChild(i);

                }

            break;
        }

    }

}

void TreeOptimiser::visit(NEndIf *node){

}

void TreeOptimiser::visit(NFunctionDeclaration *node){
    cerr << "Node: " << node->getID() << "\tRefCount: " 
         << node->getRefCount() << endl;
}

void TreeOptimiser::visit(NIdentifier *node){

}

void TreeOptimiser::visit(NInc *node){

}

void TreeOptimiser::visit(NInput *node){

}

void TreeOptimiser::visit(NInteger *node){

}

void TreeOptimiser::visit(NLoop *node){

}

void TreeOptimiser::visit(NMethodCall *node){

}

void TreeOptimiser::visit(NNullToken *node){

}

void TreeOptimiser::visit(Node *node){

}

void TreeOptimiser::visit(NParamBlock *node){

}

void TreeOptimiser::visit(NParamDeclarationBlock *node){

}

void TreeOptimiser::visit(NPrint *node){

}

void TreeOptimiser::visit(NReturn *node){

}

void TreeOptimiser::visit(NStatementList *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
        switch(node->getChild(i)->getNodeType()) {
            case FUNC:
            case PROCEDURE:
                if (node->getRefCount() == 0 && !node->isRoot()) {
                    node->removeChild(i);
                }
            break;
        }
    }

}

void TreeOptimiser::visit(NStringLit *node){

}

void TreeOptimiser::visit(NUnaryOp *node){

}

void TreeOptimiser::visit(NVariableDeclaration *node){

}

void TreeOptimiser::init(Node *node){

}



