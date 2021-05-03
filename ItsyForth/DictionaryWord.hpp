#ifndef DictionaryWord_hpp
#define DictionaryWord_hpp

#include "Cell.hpp"

class DictionaryWord {
public:
	int previous;
	char name[32];
	Instruction referenceInstruction;
	Cell* cells;
};

#endif /* DictionaryWord_hpp */
