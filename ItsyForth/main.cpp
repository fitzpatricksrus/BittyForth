#include <iostream>

#include "Runtime.hpp"
#include "Compiler.hpp"
#include "InterpreterBuilder.hpp"

int main(int argc, const char * argv[]) {
	Runtime runtime;
	InterpreterBuilder dictionary(&runtime);
	
	dictionary.reset();
	
	return 0;
}
