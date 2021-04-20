#ifndef DictionaryWord_hpp
#define DictionaryWord_hpp

#include "OpCode.hpp"

class DictionaryWord {
public:
	static const int IMMEDIATE_MASK = 0x0001;
	
	DictionaryWord* previous;
	Num flags;
	OpCode::Code opcode;
	char name[32];
	XData data[];
};

#endif /* DictionaryWord_hpp */
