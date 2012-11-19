#ifndef _NDECLARATIONBLOCK_HPP_
#define _NDECLARATIONBLOCK_HPP_

#include "Node.hpp"

class NDeclarationBlock : public Node {                                          
public:                                                                          
 vector<Node *> declarations;                                                    
};  

#endif
