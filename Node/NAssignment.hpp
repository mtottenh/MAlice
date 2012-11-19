#ifndef _NASSIGNMENT_HPP_
#define _NASSIGNMENT_HPP_
#include "Node.hpp"

#include "NExpression.hpp"
class NAssignment : public Node {
public:
 NExpression *rhs;
 Node *lhs;
 NAssignment();
};   

#endif
