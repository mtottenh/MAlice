#ifndef _NCODEBLOCK_HPP_
#define _NCODEBLOCK_HPP_

#include "NStatement.hpp"

class NCodeBlock : public NStatement {                                           
public:                                                                          
        NCodeBlock() { name = "block"; }                                         
    /* add code to add a child */                                                
};   

#endif
