//
//  Compiler.hpp
//  ItsyForth
//
//  Created by Dad on 4/30/21.
//

#ifndef Compiler_hpp
#define Compiler_hpp

#include "Runtime.hpp"

class DictionaryWord;

class Compiler {
public:
	static const int TIB_LENGTH = 256;

	Compiler(Runtime* runtime);
	
	void reset();
	int allocate(int size);

	int getDictionaryPtr() { return runtime->getCell(dictionaryPtrAddr).asData; }
	int getDictionaryPtrAddr() { return dictionaryPtrAddr; }
	void setDictionaryPtr(int addr) { runtime->setCell(dictionaryPtrAddr, addr); }
	
	int getLastWordPtr() { return runtime->getCell(lastWordPtrAddr).asData; }
	int getLastWordPtrAddr() { return lastWordPtrAddr; }
	void setLastWordPtr(int addr) { runtime->setCell(lastWordPtrAddr, addr); }

	int findWord(const std::string& name);
	int createWord(const std::string& name);
	int createWord(const std::string& name, const OpCode& opcode, int value);
	int createWord(const std::string& name, const Instruction& refInstruction);
	
	int compileInstruction(const Instruction& opcode);
	int compileLiteral(int value);
	int compileVariable(const std::string& name, int initialValue = 0);
	int compileConstant(const std::string& name, int value);
	int compileStartColonWord(const std::string& name);
	int compileData(int dataValueToCompileInline);
	int compileReference(const std::string& name);
	int compileEndWord();
	int compileBegin();
	int compileIf();
	int compileElse();
	int compileEndif();
	int compileAgain();

private:
	void pushMark(int m) { markStack[markStackPtr++] = m; }
	int popMark() { return markStack[--markStackPtr]; }

	Runtime* runtime;

	int dictionaryPtrAddr;
	int lastWordPtrAddr;
	
	int markStackPtr;
	int markStack[16];
};

#endif /* Compiler_hpp */
