#include "Runtime.hpp"
#include "DictionaryWord.hpp"
#include "OpCode.hpp"
#include <stddef.h>
#include "StructuredMemory.hpp"

Runtime::Runtime(int heapSize, int dataStackSize, int returnStackSize)
: stackPtr(0), returnPtr(0), ip(0), currentWord(0)
{
	memory = new char[heapSize];
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
	numberBase = 10;
	tibAddr = tib;
	tibContentLength = 0;
	tibInputOffset = 0;
	compilerFlags = 0;
	tib[0] = 0;

	stackPtr = 0;
	returnPtr = 0;
	ip = 0;
	currentWord = 0;
}

void Runtime::reset() {
	abortWord = 0;
	dictionaryPtr = memory;
	lastWord = 0;
	clearStacksAndIp();
}

void Runtime::abort() {
	clearStacksAndIp();
	ip = abortWord->firstInstructionPtr();
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

void Runtime::execute(DictionaryWord* newAbortWord, DictionaryWord* newIP) {
	reset();
	abortWord = newAbortWord;
	pushReturn(0L);
	ip = newIP->firstInstructionPtr();
	while (ip) {
		DictionaryWord* currentWord = consumeNextInstruction();
		currentWord->opcode.execute(this, currentWord);
	}
}

Ptr Runtime::allocate(int bytes) {
	Ptr result = dictionaryPtr;
	dictionaryPtr += bytes;
	return result;
}

