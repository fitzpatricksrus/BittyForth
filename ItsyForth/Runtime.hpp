//
//  Runtime.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef Runtime_hpp
#define Runtime_hpp

class Word;

class Runtime {
public:
	Runtime(int heapSize);
	~Runtime();
	
	void reset(int newIP);
			
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
	
	int getInstructionPointer();
	void setInstructionPointer(int newIP);
	int getCurrentWordAddr();
	
	int peekNextInstruction();
	int consumeNextInstruction();
		
	void execute(int wordAddress);
private:
	char* heap;
	int* dataStack;
	int* returnStack;
	
	int stackPtr;
	int returnPtr;
	int instructionPtr;
	int currentWord;
};

#endif /* Runtime_hpp */
