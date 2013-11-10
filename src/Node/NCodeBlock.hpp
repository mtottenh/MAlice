/* Node containing a MAlice code block. */

#ifndef _NCODEBLOCK_H_
#define _NCODEBLOCK_H_

#include "Node.hpp"
#include "NStatementList.hpp"
#include "NDeclarationBlock.hpp"

class NCodeBlock : public Node {
public:
	/* Arg 1 is a block of declarations, arg 2 is a StatementList */
	NCodeBlock(Node*, Node*);

	/* Arg 1 can be either a statement list or just a declaration block
	 * The second case is somewhat trivial as a function which
	 * declares stuff but doesnt have any real code is useless
	 * but still valid.
	 */
	NCodeBlock(Node*);
	NCodeBlock();
    virtual void accept(ASTVisitor*);
};

#endif
