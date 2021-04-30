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
	DictionaryWord* findWord(const OpCode& opcode);
	DictionaryWord* findWord(const std::string& name);
	
	DictionaryWord* createWord(OpCode opcode, Num flags = 0);
	DictionaryWord* createWord(const std::string& name, OpCode opcode, Num flags = 0);
	
	void compileOpCode(const OpCode& opcode);
	void compileConstant(const std::string& name, long value);
	void compileLiteral(long value);
	void compileStartWord(const std::string& name, Num flags = 0);
	void compileReference(DictionaryWord* word);
	void compileReference(const std::string& name);
	void compileEndWord();
	void compileBegin();
	void compileIf();
	void compileElse();
	void compileEndif();
	void compileAgain();
	
	void rebuildDictionary();
	
private:
	XData* append(const XData& data);
	void pushMarkDP();
	void pushMark(XData* m);
	XData* popMark();

	Runtime* runtime;
	XData* marks[16];
	int marksIdx;
	
};

#endif /* Builder_hpp */
