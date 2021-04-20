#include "Runtime.hpp"
#include "DictionaryWord.hpp"
#include "OpCode.hpp"
#include <stddef.h>
#include "StructuredMemory.hpp"

Runtime::Runtime(int heapSize, int dataStackSize, int returnStackSize)
: stackPtr(0), returnPtr(0), ip(0), currentWord(0)
{
	memory = new char[heapSize];
	structMemory = (StructuredMemory*)memory;
	dataStack = new XData[dataStackSize];
	returnStack = new IPtr[returnStackSize];
	reset();
}

Runtime::~Runtime() {
	delete [] memory;
	delete [] dataStack;
	delete [] returnStack;
}

void Runtime::clearStacksAndIp() {
	stackPtr = 0;
	returnPtr = 0;
	ip = 0;
	currentWord = 0;
}

void Runtime::reset() {
	structuredMemory()->reset();
	clearStacksAndIp();
}

void Runtime::abort() {
	structuredMemory()->abort();
	clearStacksAndIp();
}

XData Runtime::tos() {
	return dataStack[stackPtr - 1];
}

XData Runtime::popData() {
	return dataStack[--stackPtr];
}

void Runtime::pushData(XData data) {
	dataStack[stackPtr++] = data;
}

XData Runtime::peekData(long ndx) {
	return dataStack[stackPtr - 1 - ndx];
}

void Runtime::pokeData(long ndx, XData value) {
	dataStack[stackPtr - 1 - ndx] = value;
}

IPtr Runtime::popReturn() {
	return returnStack[--returnPtr];
}

void Runtime::pushReturn(IPtr value) {
	returnStack[returnPtr++] = value;
}

DictionaryWord* Runtime::peekNextInstruction() {
	return *ip;
}

DictionaryWord* Runtime::consumeNextInstruction() {
	DictionaryWord* result = *ip;
	ip++;
	return result;
}

IPtr Runtime::getInstructionPointer() {
	return ip;
}

void Runtime::setInstructionPointer(IPtr newIP) {
	ip = newIP;
}

DictionaryWord* Runtime::getCurrentWordAddr() {
	return currentWord;
}

StructuredMemory* Runtime::structuredMemory() {
	return structMemory;
}
	
StructuredMemory* Runtime::operator->() {
	return structMemory;
}

void Runtime::execute(IPtr newAbortIP, IPtr newIP) {
	reset();
	structMemory->abortVector = newAbortIP;
	ip = newIP;
	while (ip) {
		currentWord = consumeNextInstruction();
		DictionaryWord* w = (DictionaryWord*)currentWord;
		OpCode op(w->opcode);
		op.execute(this);
	}
}
