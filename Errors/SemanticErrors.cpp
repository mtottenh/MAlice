#include "SemanticErrors.hpp"

void error_var_exists(const string& varName) {
	cerr << "ERROR: Variable '" << varName << "' already declared in scope."
		<< endl;
}

void error_keyword(const string& keyword) {
	cerr << "ERROR: Variable '" << keyword << "' is a keyword." << endl;
}

void error_var_not_found(const string& varName) {
	cerr << "ERROR: Variable '" << varName << "' not found in scope."
		<< endl;
}

void error_type_mismatch(const string& varName, int t1, int t2) {
	cerr << "ERROR: Type mismatch. Variable '" << varName << "' of type '"
		<< typemap_get(t1) << "' does not match expected type '" << 
		typemap_get(t2) << "'" << endl;
}
