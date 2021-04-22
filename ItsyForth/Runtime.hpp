#ifndef Runtime_hpp
#define Runtime_hpp

#include <stddef.h>
#include <string.h>
#include "Types.hpp"
#include "Terminal.hpp"

class DictionaryWord;

class Runtime {
public:
	Runtime(int heapSize, int dataStackSize = 256, int returnStackSize = 256);
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

	void execute(DictionaryWord* newAbortWord, DictionaryWord* newIP /* must be colon word */);
	
	Ptr allocate(int bytes);
	
	template <typename T> T* append(T value);
	
	void emit(char c) { terminal.emit(c); }
	char read() { return terminal.read(); }

	DictionaryWord* abortWord;
	Ptr dictionaryPtr;
	Num numberBase;
	char* tibAddr;
	Num tibContentLength;
	Num tibInputOffset;
	DictionaryWord* lastWord;
	Num compilerFlags;
	char tib[256];
	
private:
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

template <typename T>
T* Runtime::append(T value) {
	T* result = (T*)allocate(sizeof(value));
	*result = value;
	return result;
}

#endif /* Runtime_hpp */
