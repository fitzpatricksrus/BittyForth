#include "OpCode.hpp"
#include "Runtime.hpp"
#include "DictionaryWord.hpp"
#include "CountedString.hpp"
#include "StructuredMemory.hpp"
#include "Builder.hpp"
#include <string>
#include <iostream>

int digitValue(int base, char c) {
	if (c >= '0' && c <= '9') {
		if (c - '0' < base) {
			return c - '0';
		} else {
			return -1;
		}
	} else if (c >= 'a' && c <= 'z') {
		if (c - 'a' < base - 10) {
			return c - 'a' + 10;
		} else {
			return -1;
		}
	} else {
		return -1;
	}
}

char* getNextWordFromInput(Runtime* runtime, char delim) {
	char* tibAddr = runtime->tibAddr;
	Num tibLength = runtime->tibContentLength;
	Num tibInOffset = runtime->tibInputOffset;
	char delimeter = runtime->popData();
	char* padAddr = runtime->dictionaryPtr;
	char* outputAddr = runtime->dictionaryPtr + 1;
	
	int len = 0;
	bool done = false;
	while (tibInOffset < tibLength && !done) {
		char c = tibAddr[tibInOffset];
		*outputAddr = c;
		if (c == delimeter) {
			done = true;
		} else {
			tibInOffset++;
			outputAddr++;
			len++;
		}
	}
	*padAddr = len;
	return padAddr;
}

int compareCountedString(char* str1, char* str2) {
	int len = *str1;
	int lenOther = *str2;
	int end = (len < lenOther) ? len : lenOther;
	for (int i = 1; i <= end; i++) {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
	}
	return len - lenOther;
}

OpCode::OpCode(OpCode::Code codeIn)
: code(codeIn)
{
}

void OpCode::execute(Runtime* runtime, DictionaryWord* currentWord) {
	switch (code) {
	case DoColon: {
		runtime->pushReturn(runtime->getInstructionPointer());
		runtime->setInstructionPointer(&(currentWord->data[0].w));
		break; }
	case DoSemicolon:
		runtime->setInstructionPointer(runtime->popReturn());
		break;
	case Constant:
	case DoConstant:
		runtime->pushData(currentWord->data[0]);
		break;
	case DoVariable:
		runtime->pushData(&currentWord->data[0]);
		break;
	case Abort:
		runtime->abort();
		break;
	case Comma: {
		runtime->append(runtime->popData());
		break; }
	case Lit:
		runtime->pushData(runtime->consumeNextInstruction());
		break;
	case Rot: {
		long temp = runtime->peekData(2);
		runtime->pokeData(2, runtime->peekData(1));
		runtime->pokeData(1, runtime->peekData(0));
		runtime->pokeData(0, temp);
		break; }
	case Drop:
		runtime->popData();
		break;
	case Dup:
		runtime->pushData(runtime->tos());
		break;
	case Swap: {
		long temp = runtime->tos();
		runtime->pokeData(0, runtime->peekData(1));
		runtime->pokeData(1, temp);
		break; }
	case Plus:
		runtime->pushData(runtime->popData().l + runtime->popData().l);
		break;
	case Equals:
		runtime->pushData(runtime->popData().l == runtime->popData().l);
		break;
	case At:
		runtime->pushData(*((Num*)runtime->popData().ptr));
		break;
	case Put: {
		Num* addr = (Num*)runtime->popData().ptr;
		*addr = runtime->popData().l;
		break; }
	case ZeroBranch: {
		IPtr destination = (IPtr)runtime->consumeNextInstruction();
		if (runtime->popData() == 0) {
			runtime->setInstructionPointer(destination);
		}
		break; }
	case Branch:
		runtime->setInstructionPointer((IPtr)runtime->consumeNextInstruction());
		break;
	case Execute:
		runtime->pushReturn(runtime->getInstructionPointer());
		runtime->setInstructionPointer((IPtr)runtime->popData().w);
		break;
	case Exit:
		runtime->setInstructionPointer(runtime->popReturn());
		break;
	case Count: {	// ( addr -- addr2 len )
					// dup 1 + swap c@
		char* addr = runtime->popData();
		runtime->pushData(addr + 1);
		runtime->pushData(*addr);
		break; }
	case ToNumber: {	//( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
		//>number - ( double addr len -- double2 addr2-zero    ) if successful, or
		//			( double addr len -- long     addr2-nonzero ) on error.
		int len = runtime->popData();
		char* addr = runtime->popData();
		Num dval = runtime->popData();
		int base = runtime->numberBase;
		bool done = false;
		while (len > 0 && !done) {
			int val = digitValue(base, *addr);
			if (val >= 0) {
				dval = dval * base + val;
				len = len - 1;
				addr = addr + 1;
			} else {
				done = true;
			}
		}
		runtime->pushData(dval);
		runtime->pushData(addr);
		runtime->pushData(len);
		break; }
	case Accept: {	//( addr len -- len2 )
		std::string str;
		std::getline(std::cin, str);
		int inputLen = (long)str.length();
		int maxLen = runtime->popData();
		maxLen = (maxLen <= inputLen) ? maxLen: inputLen;
		char* addr = (char*)runtime->popData();
		long ndx;
		for (ndx = 0; ndx < maxLen; ndx++) {
			addr[ndx] = str[ndx];
		}
		runtime->pushData(ndx);
		break; }
	case Word: { // ( char -- addr )
		char* pad = runtime->dictionaryPtr;
		char delimeter = runtime->popData();
		std::string nameIn = Builder::getNextInputWord(runtime, delimeter);
		CountedString::fromCString(nameIn, pad, padLength);
		runtime->pushData(pad);
		break; }
	case Emit:	// ( char -- )
		std::cout << (char)runtime->popData();
		std::cout.flush();
		break;
	case Find: {
		// addr -- addr2 flag  look up word in the dictionary
		// find looks in the Forth dictionary for a word with the name given in the
		// counted string at addr. One of the following will be returned:
		// flag =  0, addr2 = counted string --> word was not found
		// flag =  1, addr2 = call address   --> word is immediate
		// flag = -1, addr2 = call address   --> word is not immediate
		char* stringAddr = runtime->popData();
		DictionaryWord* thisWordAddr = runtime->lastWord;
		bool done = false;
		while (thisWordAddr != 0 && !done) {
			char* thisWordNameAddr = thisWordAddr->name;
			
			if (CountedString::compare(stringAddr, thisWordNameAddr) != 0) {
				thisWordAddr = thisWordAddr->previous;
			} else {
				done = true;
			}
		}
		if (thisWordAddr != 0) {
			runtime->pushData(thisWordAddr);
			runtime->pushData(thisWordAddr->flags & DictionaryWord::IMMEDIATE_MASK);
		} else {
			runtime->pushData(stringAddr);
			runtime->pushData(0L);
		}
		break; }
	case SemiColon:
		runtime->setInstructionPointer(runtime->popReturn());
		break;
	case Colon:	{	// compiling word
		assert(runtime->compilerFlags == 0);
		// set state to compile
		runtime->compilerFlags = -1;
		std::string nameIn = Builder::getNextInputWord(runtime);
		Builder::create(runtime, nameIn, OpCode::Colon, 0);
		break; }
	case Create: {
		// -- create a new word --
		std::string nameIn = Builder::getNextInputWord(runtime);
		Builder::create(runtime, nameIn, OpCode::Colon, 0);
		break; }
	case SemiCode:
		assert(runtime->compilerFlags != 0);
		// clear state to compile
		runtime->compilerFlags = 0;
		break;
	default: {
		break; }
	}
}


