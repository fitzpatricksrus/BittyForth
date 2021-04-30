#ifndef Runtime_hpp
#define Runtime_hpp

#include <stddef.h>
#include <string.h>
#include "Types.hpp"
#include "Terminal.hpp"

class DictionaryWord;

class Runtime {
public:
//	void dbg(long addr, std::string msg);
	void dbg(void* addr, std::string msg);
	void dbg(std::string msg);
	long dbgOffset(void* addr);

	Runtime(int heapSize = 16384, int dataStackSize = 256, int returnStackSize = 256);
	~Runtime();
	
	void reset();
	void abort();
	
	XData tos();
	XData popData();
	void pushData(XData data);
	XData peekData(long ndx);
	void pokeData(long ndx, XData value);
	
	IPtr popReturn();
	void pushReturn(IPtr value);
	
	DictionaryWord* peekNextInstruction();
	DictionaryWord* consumeNextInstruction();
	IPtr getInstructionPointer();
	void setInstructionPointer(IPtr newIP);

	void execute(DictionaryWord* newAbortWord, DictionaryWord* wordToExecute /* must be colon word */);
	
	void emit(char c) { terminal.emit(c); }
	char read() { return terminal.read(); }

	XData* allocate(int bytes);
	
	DictionaryWord* getAbortWordPtr() { return *abortWordPtrAddr; }
	DictionaryWord** getAbortWordPtrAddr() { return abortWordPtrAddr; }
	void setAbortWordPtr(DictionaryWord* ptr) { *abortWordPtrAddr = ptr; }

	char* getDictionaryPtr() { return *dictionaryPtrAddr; }
	char** getDictionaryPtrAddr() { return dictionaryPtrAddr; }
	void setDictionaryPtr(char* ptr) { *dictionaryPtrAddr = ptr; }
	
	DictionaryWord* getLastWordPtr() { return *lastWordPtrAddr; }
	DictionaryWord** getLastWordPtrAddr() { return lastWordPtrAddr; }
	void setLastWordPtr(DictionaryWord* ptr) { *lastWordPtrAddr = ptr; }
	
	Num getNumberBase() { return *numberBaseAddr; }
	Num* getNumberBaseAddr() { return numberBaseAddr; }
	void setNumberBase(Num ptr) { *numberBaseAddr = ptr; }
	
	Num getTibContentLength() { return *tibContentLengthAddr; }
	Num* getTibContentLengthAddr() { return tibContentLengthAddr; }
	void setTibContentLength(Num ptr) { *tibContentLengthAddr = ptr; }
	
	Num getTibInputOffset() { return *tibInputOffsetAddr; }
	Num* getTibInputOffsetAddr() { return tibInputOffsetAddr; }
	void setTibInputOffset(Num ptr) { *tibInputOffsetAddr = ptr; }
	
	Num getCompilerFlags() { return *compilerFlagsAddr; }
	Num* getCompilerFlagsAddr() { return compilerFlagsAddr; }
	void setCompilerFlags(Num ptr) { *compilerFlagsAddr = ptr; }
	
	char* getTibAddr() { return *tibAddrAddr; }
	void setTibAddr(char* addr) { *tibAddrAddr = addr; };

	char** getTibAddrAddr() { return tibAddrAddr; }

private:
	DictionaryWord** abortWordPtrAddr;
	char** dictionaryPtrAddr;
	DictionaryWord** lastWordPtrAddr;
	Num* numberBaseAddr;
	Num* tibContentLengthAddr;
	Num* tibInputOffsetAddr;
	Num* compilerFlagsAddr;
	char** tibAddrAddr;

	void clearStacksAndIp();

	char* memory;
	XData* dataStack;
	IPtr* returnStack;
	
	int stackPtr;
	int returnPtr;
	IPtr ip;
	DictionaryWord* currentWord;
	Terminal terminal;
};

#endif /* Runtime_hpp */
