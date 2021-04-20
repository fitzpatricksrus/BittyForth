//
//  Runtime.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef Runtime_hpp
#define Runtime_hpp

#include <stddef.h>
#include "Types.hpp"

class DictionaryWord;
class StructuredMemory;

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
	DictionaryWord* getCurrentWordAddr();
	
	StructuredMemory* structuredMemory();
	StructuredMemory* operator->();
		
	void execute(DictionaryWord* newAbortIP, IPtr newIP);
	
private:
	void clearStacksAndIp();

	char* memory;
	StructuredMemory* structMemory;		//alias of 'memory' that can be treated like a c struct
	XData* dataStack;
	IPtr* returnStack;
	
	int stackPtr;
	int returnPtr;
	IPtr ip;
	DictionaryWord* currentWord;
};

#endif /* Runtime_hpp */
