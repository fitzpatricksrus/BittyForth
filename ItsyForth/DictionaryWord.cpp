#include "DictionaryWord.hpp"

#include "Runtime.hpp"

DictionaryWord* DictionaryWord::create(Runtime* runtime, const std::string& name, OpCode opcode) {
	DictionaryWord* result = (DictionaryWord*)runtime->allocate(sizeof(DictionaryWord));
	result->previous = runtime->lastWord;
	runtime->lastWord = result;
	result->flags = 0;
	result->opcode = opcode;
	
}
