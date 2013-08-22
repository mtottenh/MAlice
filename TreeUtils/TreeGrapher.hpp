#ifndef __TREE_GRAPH_H__
#define __TREE_GRAPH_H__
#include "../CodeGeneration/ASTVisitor.hpp"
#include <boost/graph/adjacency_list.hpp>

	
struct Vertex{ std::string node_name; std::string type;};

//Define the graph using those classes
typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS, Vertex> Graph;
//Some typedefs for simplicity
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;



class TreeGrapher : public ASTVisitor {
private:

    Graph g;
public:
    TreeGrapher();  
    string getAssembly() { return NULL; };
    void generateFunctionDefinitions(){} ; 
    void init(Node*, CodeGenerator*){} ; 
    void visit(NArrayAccess*) ;
    void visit(NAssignment*);
    void visit(NBinOp*);
    void visit(NCharLit*);
    void visit(NCodeBlock*);
    void visit(NConditional*);
    void visit(NDec*);
    void visit(NDeclarationBlock*);
    void visit(NEndIf*);
    void visit(NFunctionDeclaration*);
    void visit(NIdentifier*);
    void visit(NInc*);
    void visit(NInput*);
    void visit(NInteger*);
    void visit(NLoop*);
    void visit(NMethodCall*);
    void visit(NNullToken*);
    void visit(Node*);
    void visit(NParamBlock*);
    void visit(NParamDeclarationBlock*);
    void visit(NPrint*);
    void visit(NReturn*);
    void visit(NStatementList*);
    void visit(NStringLit*);
    void visit(NUnaryOp*);
    void visit(NVariableDeclaration*);
};

#endif
