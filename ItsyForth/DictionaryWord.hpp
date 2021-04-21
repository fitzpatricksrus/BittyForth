#ifndef DictionaryWord_hpp
#define DictionaryWord_hpp

#include "OpCode.hpp"
#include <string>
#include "Types.hpp"

class DictionaryWord {
public:
	static const int IMMEDIATE_MASK = 0x0001;
	
	DictionaryWord* previous;
	Num flags;
	OpCode opcode;
	char name[32];
	XData data[];
	
	IPtr firstInstructionPtr() { return (IPtr)data; }
};

#endif /* DictionaryWord_hpp */
