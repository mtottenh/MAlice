#ifndef _NFUNCTIONDECLARATION_HPP_
#define _NFUNCTIONDECLARATION_HPP_

#include "Node.hpp"
#include "NIdentifier.hpp"
                                                                                 
class NFunctionDeclaration : public Node {                          
                                                                                 
public:                                                                          
        NFunctionDeclaration();                           
	NFunctionDeclaration(NIdentifier*, Node*, Node* , int);
	NFunctionDeclaration(NIdentifier*, Node*, int);
	NFunctionDeclaration(NIdentifier*, Node*, Node*);
	NFunctionDeclaration(NIdentifier*, Node*);
	int print() const;
};    

#endif
