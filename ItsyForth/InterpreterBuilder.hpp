//
//  InterpreterBuilder.hpp
//  ItsyForth
//
//  Created by Dad on 5/1/21.
//

#ifndef InterpreterBuilder_hpp
#define InterpreterBuilder_hpp

#include "Compiler.hpp"
class Runtime;

class InterpreterBuilder {
public:
	static const int TIB_LENGTH = 256;

	InterpreterBuilder(Runtime* runtime);
	
	void reset();
	
private:
	DictionaryWord* create(const std::string& name, const Instruction& refIns);

	Runtime* runtime;
	Compiler compiler;
};

#endif /* InterpreterBuilder_hpp */
