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
