//
//  Builder.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef Builder_hpp
#define Builder_hpp

#include <string>
#include "Types.hpp"

class Runtime;
class DictionaryWord;
class OpCode;

class Builder {
public:
	static std::string getNextInputWord(Runtime* runtime, char delimeter = ' ');
	static DictionaryWord* create(Runtime* runtime, const std::string& name, OpCode opcode, Num flags = 0);

	static void rebuildDictionary(Runtime* runtime);
};

#endif /* Builder_hpp */
