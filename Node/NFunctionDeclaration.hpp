#ifndef _NFUNCTIONDECLARATION_HPP_
#define _NFUNCTIONDECLARATION_HPP_

#include "Node.hpp"

                                                                                 
class NFunctionDeclaration : public Node {                          
                                                                                 
public:                                                                          
        NFunctionDeclaration();                           
        NFunctionDeclaration(Node*);
	int print() const;
};    

#endif
