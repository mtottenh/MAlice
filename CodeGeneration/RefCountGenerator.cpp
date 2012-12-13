#include "RefCountGenerator.hpp"

/* Reference counter generator
 * looks for function calls in the AST
 * once found it incrments a reference count field 
 * of the declaration node a second visitor will 
 * then remove all Function  Declaration nodes with
 * a ref count of 0
 */

RefCountGenerator::RefCountGenerator() {

}

void RefCountGenerator::visit(NArrayAccess *node) {

}

void RefCountGenerator::visit(NAssignment *node){ 
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}

void RefCountGenerator::visit(NBinOp *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }


}

void RefCountGenerator::visit(NCharLit *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NCodeBlock *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NConditional *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NDec *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NDeclarationBlock *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NEndIf *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NFunctionDeclaration *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NIdentifier *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NInc *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }

}


void RefCountGenerator::visit(NInput *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}

void RefCountGenerator::visit(NInteger *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NLoop *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NMethodCall *node){
    Node* DeclarationNode = node->getTable()->lookup(node->getID());
    DeclarationNode->incRefCount();
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NNullToken *node){

}


void RefCountGenerator::visit(Node *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NParamBlock *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NParamDeclarationBlock *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NPrint *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NReturn *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NStatementList *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NStringLit *node){

}


void RefCountGenerator::visit(NUnaryOp *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::visit(NVariableDeclaration *node){
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}


void RefCountGenerator::init(Node *node){

        } 



