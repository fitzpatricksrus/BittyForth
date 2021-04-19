//
//  Runtime.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef Runtime_hpp
#define Runtime_hpp

#include <stddef.h>

class Word;
class BaseMemory;

class Runtime {
public:
	Runtime(int heapSize, int dataStackSize = 256, int returnStackSize = 256);
	~Runtime();
	
	void reset();
			
	int getInt(int addr);
	void setInt(int addr, int value);
	char getChar(int addr);
	void setChar(int addr, char value);
	
	int tos();
	int popData();
	void pushData(int data);
	int peekData(int ndx);
	void pokeData(int ndx, int value);
	
	int popReturn();
	void pushReturn(int value);
	
	int peekNextInstruction();
	int consumeNextInstruction();
	int getInstructionPointer();
	void setInstructionPointer(int newIP);
	int getCurrentWordAddr();
	
	char* toAnsoluteAddr(int addr);
	int toRelativeAddr(char* ptr);
	BaseMemory* baseMemory() { return (BaseMemory*)heap; }
		
	void execute(int newAbortIP, int newIP);
	
private:
	char* heap;
	int* dataStack;
	int* returnStack;
	
	int stackPtr;
	int returnPtr;
	int ip;
	int currentWord;
};

#endif /* Runtime_hpp */
