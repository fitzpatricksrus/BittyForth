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
	Builder(Runtime* runtime);
	
	std::string getNextInputWord(char delim);
	DictionaryWord* findWord(const std::string& name);
	
	DictionaryWord* createWord(OpCode opcode, Num flags = 0);
	DictionaryWord* createWord(const std::string& name, OpCode opcode, Num flags = 0);
	XPtr append(long value);
	XPtr append(void* addr);
	XPtr append(const XData& data);
	XPtr append(const std::string& wordName);
	
	void compileBegin();
	void compileIf();
	void compileElse();
	void compileEndif();
	void compileAgain();
	
private:
	void pushMark();
	void pushMark(XPtr m);
	XPtr popMark();

	Runtime* runtime;
	XPtr marks[16];
	int marksIdx;
	
};

#endif /* Builder_hpp */
