#include "SemanticErrors.hpp"

void error_var_exists(const string& varName) {
	cerr << "ERROR: Variable '" << varName << "' already declared in scope."
		<< endl;
}

void error_keyword(const string& keyword) {
	cerr << "ERROR: Variable '" << keyword << "' is a keyword." << endl;
}

void error_var_not_found(const string& varName) {
	cerr << "ERROR: Identifier, variable or method '" << varName << 
		"' not found in scope." << endl;
}

void error_type_mismatch(const string& varName, int t1, int t2) {
	cerr << "ERROR: Type mismatch. Variable '" << varName << "' of type '"
		<< typemap_get(t1) << "' does not match expected type '" << 
		typemap_get(t2) << "'" << endl;
}

void error_type_mismatch(int op, int t1, int t2) {
	cerr << "ERROR: Type mismatch. Operator '" << typemap_get(op) 
		<< "' was given an expression of type '" << typemap_get(t1) 
		<< "' which does not match expected type '" << typemap_get(t2) 
		<< "'" << endl;
}

void error_type_mismatch(int op, int t1, const string& types) {
	cerr << "ERROR: Type mismatch. Operator '" << typemap_get(op) 
		<< "' was given an expression of type '" << typemap_get(t1) 
		<< "' which does not match an expected type '" 
		<< types << "'" << endl;

}

void error_type_mismatch_cond(int t1) {
	cerr << "ERROR: Type mismatch. Conditional expected a predicate, but "
		<< "got an expression of type " << t1 << endl;
}

void error_no_entry() {
	cerr << "ERROR: No program entry point (procedure 'hatta') detected."
		<< endl;
}

void error_not_func(const string& name) {
	cerr << "ERROR: '" << name << "' is not a function or procedure." 
		<< endl;
}

void error_num_args(const string& name, int expected, int given) {
	cerr << "ERROR: Function or procedure '" << name 
		<< "' is of arity " << expected << " but was given " << given 
		<< " arguments." << endl;
}

void error_num_args(int typeVal, int expected, int given) {
	cerr << "ERROR: Operator '" << typemap_get(typeVal) << "' is of arity "
		<< expected << " but was given " << given << " operands." 
		<< endl;

}

void error_input_type(const string& name, int typeVal) {
	cerr << "ERROR: Variable '" << name << "' of type '" << 
		typemap_get(typeVal) << "' cannot be given user input." << endl;
}

void error_not_array(const string& name, int typeVal) {
	cerr << "ERROR: Variable '" << name << "' of type '" <<
		typemap_get(typeVal) << "' is not an array." << endl;
}

void error_entry_not_proc() {
	cerr << "ERROR: A program entry point ('hatta') was detected, but it "
		<< "is a function. The entry point should be a procedure."
		<< endl;
}
