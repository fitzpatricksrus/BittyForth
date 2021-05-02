//
//  Instruction.hpp
//  ItsyForth
//
//  Created by Dad on 5/1/21.
//

#ifndef Instruction_hpp
#define Instruction_hpp

#include "OpCode.hpp"
#include <string>

class Runtime;

class Instruction {
public:
	Instruction(const OpCode& opcodeIn) :opcode(opcodeIn), data(-1) {}
	Instruction(const OpCode& opcodeIn, int dataIn) :opcode(opcodeIn), data(dataIn) {}

	OpCode opcode;
	int	data;
	
	void execute(Runtime* runtime, int currentInstructionPtr, int nextInstructionPtr);
	
	std::string toString() const;
};

#endif /* Instruction_hpp */
