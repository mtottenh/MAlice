/* Node representing an identifier in the MAlice language. */

#ifndef _NIDENTIFIER_HPP_
#define _NIDENTIFIER_HPP_

#include "Node.hpp"
#include <string>

class NIdentifier : public Node {
public:
		/* Empty constructor. */
       	NIdentifier();
		
		/* Sets the identifiers value to an id given by a string or char* */
		NIdentifier(string);
	    NIdentifier(char*);

		/* Return the data type of the node. */
		int getType();

		/* Returns the identifier of the... identifier. */  
		string getID();
        
		/*                                                                           
 		 * Checks that the node is semantically valid. Returns 1 if this is the      
 		 * case, 0 otherwise.                                                        
 		 */       
		virtual int check(); 
	
};  

#endif
