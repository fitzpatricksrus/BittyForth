//
//  Runtime.cpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#include "Runtime.hpp"
#include "Word.hpp"
#include "OpCode.hpp"
#include <stddef.h>
#include "BaseMemory.hpp"

Runtime::Runtime(int heapSize, int dataStackSize, int returnStackSize)
: stackPtr(0), returnPtr(0), ip(0), currentWord(0)
{
	heap = new char[heapSize];
	dataStack = new int[dataStackSize];
	returnStack = new int[returnStackSize];
	reset();
}

Runtime::~Runtime() {
	delete [] heap;
	delete [] dataStack;
	delete [] returnStack;
}

void Runtime::reset() {
	baseMemory()->reset();
	stackPtr = 0;
	returnPtr = 0;
	ip = 0;
	currentWord = 0;
}

int Runtime::getInt(int addr) {
	return *(int*)(heap + addr);
}

void Runtime::setInt(int addr, int value) {
	*(int*)(heap + addr) = value;
}

char Runtime::getChar(int addr) {
	return heap[addr];
}

void Runtime::setChar(int addr, char value) {
	heap[addr] = value;
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

int Runtime::peekNextInstruction() {
	return getInt(ip);
}

int Runtime::consumeNextInstruction() {
	int result = getInt(ip);
	ip += sizeof(int);
	return result;
}

int Runtime::getInstructionPointer() {
	return ip;
}

void Runtime::setInstructionPointer(int newIP) {
	ip = newIP;
}

int Runtime::getCurrentWordAddr() {
	return currentWord;
}

char* Runtime::toAnsoluteAddr(int addr) {
	return heap + addr;
}

int Runtime::toRelativeAddr(char* ptr) {
	return (int)(ptr - heap);
}

void Runtime::execute(int newAbortIP, int newIP) {
	reset();
	baseMemory()->abortVector = newAbortIP;
	ip = newIP;
	while (ip != 0) {
		currentWord = consumeNextInstruction();
		Word* w = (Word*)toAnsoluteAddr(currentWord);
		OpCode op(w->opcode);
		op.execute(this);
	}
}


