#ifndef _NASSIGNMENT_HPP_
#define _NASSIGNMENT_HPP_

#include "NStatement.hpp"
#include "NExpression.hpp"
class NAssignment : public NStatement {                                          
public:                                                                          
 NExpression *rhs;                                                               
 Node *lhs;                                                                      
 NAssignment(); 
};   

#endif
