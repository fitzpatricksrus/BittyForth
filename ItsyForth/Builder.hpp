//
//  Builder.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef Builder_hpp
#define Builder_hpp

#include <string>

class Runtime;
class Word;
class OpCode;

class Builder {
public:
	Builder(Runtime* runtime);
	
	Word* allocateWord(std::string str, OpCode* op, int size = 0);
	Word* allocateImmediateWord(std::string str, OpCode* op, int size = 0);
	char* allocateBytes(int size);
	void append(int value);
	void append(Word* value);
	
	//int dp;	//at location 0 in runtime
	
private:
	Runtime* runtime;
};

#endif /* Builder_hpp */
