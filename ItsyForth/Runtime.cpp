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
	return getInt(getInstructionPointer());
}

int Runtime::consumeNextInstruction() {
	int result = getInt(getInstructionPointer());
	setInstructionPointer(getInstructionPointer()+1);
	return result;
}

void Runtime::execute(int wordAddress) {
	reset(wordAddress + (int)offsetof(struct Word, data));
	int ip = getInstructionPointer();
	while (ip != 0) {
		currentWord = consumeNextInstruction();
		OpCode op(getInt(currentWord + (int)offsetof(struct Word, opcode)));
		op.execute(this);
	}
}



