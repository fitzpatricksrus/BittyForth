#ifndef Runtime_hpp
#define Runtime_hpp

#include <stddef.h>
#include <string.h>
#include "Terminal.hpp"
#include "Instruction.hpp"
#include "Cell.hpp"

class Runtime {
public:
	static const int heapSize = 16384;
	static const int dataStackSize = 256;
	static const int returnStackSize = 256;

	void dbg(int addr, std::string msg);
	void dbg(std::string msg);

	Runtime();
	~Runtime();
	
	void execute();
	void* asPtr(int addr);
	
	Cell& getCell(int addr);
	void setCell(int addr, int data);
	void setCell(int addr, const Instruction& data);
	void setCell(int addr, Cell data);
	char& getByte(int addr);
	void setByte(int addr, char value);
	
	char& operator()(int addr) { return *(memory + addr); }
	Cell& operator[](int addr) { return (Cell&) *asPtr(addr); }
	
	int tos();
	int popData();
	void pushData(int data);
	int peekData(int ndx);
	void pokeData(int ndx, int value);
	
	int popReturn();
	void pushReturn(int value);
	
	int getCurrentInstructionPointer();
	int getNextInstructionPointer();
	void setNextInstructionPointer(int newIP);
	
	void emit(char c) { terminal.emit(c); }
	char read() { return terminal.read(); }
	
private:
	char* memory;
	int* dataStack;
	int* returnStack;	//this is cells so data can be put here
	
	int stackPtr;
	int returnPtr;
	int nextInstructionPtr;
	int currentInstructionPtr;
	Terminal terminal;
};

template <typename T>
T* Runtime::getAddr(int addr) {
	return (T*)memory;
}


#endif /* Runtime_hpp */
