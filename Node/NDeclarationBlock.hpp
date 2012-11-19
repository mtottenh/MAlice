#ifndef _NDECLARATIONBLOCK_HPP_
#define _NDECLARATIONBLOCK_HPP_

#include "NStatement.hpp"
class NDeclarationBlock : public NStatement {  
public:	
	NDeclarationBlock(){};
	NDeclarationBlock(NDeclarationBlock *derp){ children.push_back(derp);};  
};  

#endif
