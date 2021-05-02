#include "Runtime.hpp"
#include "Debug.hpp"

void Runtime::dbg(int addr, std::string msg) {
	Debug::print(">> " + std::to_string(addr) + ": " + msg);
}

void Runtime::dbg(std::string msg) {
	Debug::print(">> " + msg);
}

Runtime::Runtime()
: stackPtr(0), returnPtr(0), nextInstructionPtr(0), currentInstructionPtr(0)
{
	memory = new char[heapSize];
	dataStack = new int[dataStackSize];
	returnStack = new int[returnStackSize];
}

Runtime::~Runtime() {
	delete [] memory;
	delete [] dataStack;
	delete [] returnStack;
}

void Runtime::execute() {
	stackPtr = 0;
	returnPtr = 0;
	currentInstructionPtr = 0;
	nextInstructionPtr = sizeof(Cell);
	
	currentInstructionPtr = nextInstructionPtr++;
	Cell& cell = getCell(currentInstructionPtr);
	do {
		cell.asInstruction.execute(this, currentInstructionPtr, nextInstructionPtr);
		currentInstructionPtr = nextInstructionPtr++;
		cell = getCell(currentInstructionPtr);
	} while (cell.asInstruction.opcode != OpCode::Exit);
}

void* Runtime::asPtr(int addr) {
	return memory + addr;
}

Cell& Runtime::getCell(int addr) {
	Cell* result = (Cell*)(memory + addr);
	return *result;
}

void Runtime::setCell(int addr, int data) {
	Cell* result = (Cell*)(memory + addr);
	result->asData = data;
}

void Runtime::setCell(int addr, const Instruction& data) {
	Cell* result = (Cell*)(memory + addr);
	result->asInstruction = data;
}

void Runtime::setCell(int addr, Cell data) {
	Cell* result = (Cell*)(memory + addr);
	*result = data;
}

char& Runtime::getByte(int addr) {
	return memory[addr];
}

void Runtime::setByte(int addr, char value) {
	memory[addr] = value;
}

int Runtime::tos() {
	return dataStack[stackPtr - 1];
}

int Runtime::popData() {
	return dataStack[--stackPtr];
}

void Runtime::pushData(int data) {
	dataStack[stackPtr++] = data;
}

int Runtime::peekData(int ndx) {
	return dataStack[stackPtr - 1 - ndx];
}

void Runtime::pokeData(int ndx, int value) {
	dataStack[stackPtr - 1 - ndx] = value;
}

int Runtime::popReturn() {
	return returnStack[--returnPtr];
}

void Runtime::pushReturn(int value) {
	returnStack[returnPtr++] = value;
}

int Runtime::getCurrentInstructionPointer() {
	return currentInstructionPtr;
}

int Runtime::getNextInstructionPointer() {
	return nextInstructionPtr;
}

void Runtime::setNextInstructionPointer(int newIP) {
	nextInstructionPtr = newIP;
}
