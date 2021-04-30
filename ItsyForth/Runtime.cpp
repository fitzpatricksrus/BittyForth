#include "Runtime.hpp"
#include "DictionaryWord.hpp"
#include "OpCode.hpp"
#include <stddef.h>
#include "StructuredMemory.hpp"
#include "Builder.hpp"
#include "CoreDictionary.hpp"
#include "Debug.hpp"

//void Runtime::dbg(long value, std::string msg) {
//	Debug::print(">> " + std::to_string(value) + ": " + msg);
//}

void Runtime::dbg(void* addr, std::string msg) {
	Debug::print(">> " + std::to_string((long)(((char*)addr) - memory)) + ": " + msg);
}

void Runtime::dbg(std::string msg) {
	Debug::print(">> " + msg);
}

long Runtime::dbgOffset(void* addr) {
	return ((char*)addr) - memory;
}

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
	numberBaseAddr = (Num*)allocate(sizeof(*numberBaseAddr));
		dbg(numberBaseAddr, "numberBaseAddr");
	tibContentLengthAddr = (Num*)allocate(sizeof(*tibContentLengthAddr));
		dbg(tibContentLengthAddr, "tibContentLengthAddr");
	tibInputOffsetAddr = (Num*)allocate(sizeof(*tibInputOffsetAddr));
		dbg(tibInputOffsetAddr, "tibInputOffsetAddr");
	compilerFlagsAddr = (Num*)allocate(sizeof(*compilerFlagsAddr));
		dbg(compilerFlagsAddr, "compilerFlagsAddr");
	char* tibAddr = (char*)allocate(sizeof(char) * 256);
		dbg(tibAddr, "tibAddr");
	tibAddrAddr = (char**)allocate(sizeof(*tibAddrAddr));
		dbg(tibAddrAddr, "tibAddrAddr");

	*numberBaseAddr = 10;
	*tibContentLengthAddr = 0;
	*tibInputOffsetAddr = 0;
	*compilerFlagsAddr = 0;
	tibAddr[0] = 0;
	*tibAddrAddr = tibAddr;

	stackPtr = 0;
	returnPtr = 0;
	ip = 0;
	currentWord = 0;
}

void Runtime::reset() {
	dictionaryPtrAddr = (char**)memory;
		dbg(dictionaryPtrAddr, "dictionaryPtrAddr");
	*dictionaryPtrAddr = memory + sizeof(*dictionaryPtrAddr);
	abortWordPtrAddr = (DictionaryWord**)allocate(sizeof(*abortWordPtrAddr));
		dbg(abortWordPtrAddr, "abortWordPtrAddr");
	lastWordPtrAddr = (DictionaryWord**)allocate(sizeof(*lastWordPtrAddr));
		dbg(lastWordPtrAddr, "lastWordPtrAddr");
	clearStacksAndIp();
	
	Builder(this).rebuildDictionary();
}

void Runtime::abort() {
	clearStacksAndIp();
	ip = (*abortWordPtrAddr)->firstInstructionPtr();
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

void Runtime::execute(DictionaryWord* newAbortWord, DictionaryWord* wordToExecute) {
	(*abortWordPtrAddr) = newAbortWord;
	pushReturn(0L);
	ip = wordToExecute->firstInstructionPtr();
	while (ip) {
		DictionaryWord* currentWord = consumeNextInstruction();
		currentWord->opcode.execute(this, currentWord);
	}
}

XData* Runtime::allocate(int bytes) {
	XData* result = (XData*)dictionaryPtrAddr;
	Ptr addr = (*dictionaryPtrAddr);
	addr += bytes;
	(*dictionaryPtrAddr) = addr;
 	return result;
}
