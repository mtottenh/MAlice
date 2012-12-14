#include "TreeOptimiser.hpp"
#include "../Node/NodeIncludes.hpp"
TreeOptimiser::TreeOptimiser() {

}

void TreeOptimiser::visit(NArrayAccess *node){
    for (int i = 0; i < node->getChildrenSize(); i++) {
        node->getChild(i)->accept(this);
    }

}

void TreeOptimiser::visit(NAssignment *node){
    Node  *replacement, *lexp, *rexp;
    NBinOp *value;
    if (node->getChildrenSize() > 1) {
        value = (NBinOp *)node->getChild(1);
        if (value->getNodeType() == BINOP) {
            lexp = value->getChild(0);
            rexp = value->getChild(1);
           if ( (lexp->getNodeType() == INTEGER ) 
               && (rexp->getNodeType() == INTEGER) ) {
               switch(value->getOp()) {
                    case AND:
                         replacement = new NInteger( ((NInteger *)lexp)->getValue() 
                                            & ((NInteger *)rexp)->getValue());
                         node->removeChild(1);
                         node->addChild(replacement);
                         break;
                    case OR:
                    replacement = new NInteger( ((NInteger *)lexp)->getValue() 
                                            | ((NInteger *)rexp)->getValue());
                         node->removeChild(1);
                         node->addChild(replacement);
                        break;
                    case XOR:
                    replacement = new NInteger( ((NInteger *)lexp)->getValue() 
                                            ^ ((NInteger *)rexp)->getValue());
                         node->removeChild(1);
                         node->addChild(replacement);
                        break;
                    case PLUS:
                    replacement = new NInteger( ((NInteger *)lexp)->getValue() 
                                            + ((NInteger *)rexp)->getValue());
                         node->removeChild(1);
                         node->addChild(replacement);
                        break;
                    case DASH:
                    replacement = new NInteger( ((NInteger *)lexp)->getValue() 
                                            - ((NInteger *)rexp)->getValue());
                         node->removeChild(1);
                         node->addChild(replacement);
                        break;
               }

           }
        }   

    }
}

void TreeOptimiser::visit(NBinOp *node){

}

void TreeOptimiser::visit(NCharLit *node){

}

void TreeOptimiser::visit(NCodeBlock *node){
    for (int i = 0; i < node->getChildrenSize(); i++) {
        node->getChild(i)->accept(this);
    }

}

void TreeOptimiser::visit(NConditional *node){

}

void TreeOptimiser::visit(NDec *node){

}

void TreeOptimiser::visit(NDeclarationBlock *node){
    int i = 0;
    int size = node->getChildrenSize();
    while(i < size) {
        
        Node *child = node->getChild(i);
//        cerr << "Testing Node[" << i << "] : " << child->getID() << endl;
        switch(child->getNodeType()) {
            case FUNC:
            case PROCEDURE:
          //      cerr << "Testing function for removal candidate in DecBlock\n";
        //        cerr << "Function Reference Count: " << child->getRefCount() << endl;
      //          cerr << "Testing Function body" << endl;
                child->accept(this);
                if (child->getRefCount() == 0 
                    && child->getID() != "hatta") {
                    cerr << "Function: "<< child->getID() 
                         << " removed from AST\n";   
                    node->removeChild(i);
                    i = 0;
                } else {
                    i++;
                }
                break;
            default:
//                cerr << "Node is not a function, continuing" << endl;
                i++;
                
        }
        size = node->getChildrenSize();

  //      cerr << "Node children count : " << size << endl;
    //    cerr << "value of index: " << i << endl;

    }

}

void TreeOptimiser::visit(NEndIf *node){

}

void TreeOptimiser::visit(NFunctionDeclaration *node){
    for (int i = 0; i < node->getChildrenSize(); i++) {
        node->getChild(i)->accept(this);

    }


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



