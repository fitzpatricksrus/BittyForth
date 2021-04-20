#include "OpCode.hpp"
#include "Runtime.hpp"
#include "DictionaryWord.hpp"
#include "CountedString.hpp"
#include "StructuredMemory.hpp"
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
	char* tibAddr = (*runtime)->tibPtr;
	Num tibLength = (*runtime)->tibContentLength;
	Num tibInOffset = (*runtime)->tibInputOffset;
	char delimeter = runtime->popData();
	char* padAddr = (*runtime)->dictionaryPtr;
	char* outputAddr = (*runtime)->dictionaryPtr + 1;
	
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

int compareCountedString(Runtime* runtime, char* str1, char* str2) {
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

void OpCode::execute(Runtime* runtime) {
	switch (code) {
	case DoColon:
		runtime->pushReturn(runtime->getInstructionPointer());
		DictionaryWord* currentWord = runtime->getCurrentWordAddr();
		runtime->setInstructionPointer(&(currentWord->data[0].w));
		break;
	case DoSemicolon:
		runtime->setInstructionPointer(runtime->popReturn());
		break;
	case DoConstant:
		runtime->pushData(runtime->getCurrentWordAddr()->data[0]);
		break;
	case DoVariable:
		runtime->pushData((Num)&(runtime->getCurrentWordAddr()->data[0]));
		break;
	case Abort:
		runtime->abort();
		break;
	case Comma: {
		runtime->structuredMemory()->append(runtime->popData());
		break; }
	case Lit:
		runtime->pushData((Num)runtime->consumeNextInstruction());
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
		char* addr = (char*)runtime->popData();
		Num dval = runtime->popData();
		int base = (*runtime)->numberBase;
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
		char* tibAddr = (*runtime)->tibPtr;
		char* inputAddr = tibAddr + (*runtime)->tibInputOffset;
		char* inputEndAddr = tibAddr + (*runtime)->tibContentLength;
		char* padAddr = (*runtime)->dictionaryPtr;
		char* outputAddr = padAddr + 1;	//leave room for length byte
		char delimeter = runtime->popData();
		
		//skip leading delimeters
		while ((inputAddr < inputEndAddr) && (*inputAddr == delimeter)) {
			inputAddr++;
		}
		
		while ((inputAddr < inputEndAddr) && (*inputAddr != delimeter)) {
			*outputAddr = *inputAddr;
			outputAddr++;
			inputAddr++;
		}
		
		*padAddr = outputAddr - (padAddr + 1);
		runtime->pushData((Num)padAddr);
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
		StructuredMemory* mem = runtime->structuredMemory();
		char* stringAddr = (char*)runtime->popData();
		DictionaryWord* thisWordAddr = mem->lastWord;
		bool done = false;
		while (thisWordAddr != 0 && !done) {
			char* thisWordNameAddr = &(thisWordAddr->name[0]);
			
			if (compareCountedString(runtime, stringAddr, thisWordNameAddr) != 0) {
				thisWordAddr = thisWordAddr->previous;
			} else {
				done = true;
			}
		}
		if (thisWordAddr != 0) {
			runtime->pushData(thisWordAddr);
			runtime->pushData(((*runtime)(thisWordAddr + (long)offsetof(struct DictionaryWord, flags))) & DictionaryWord::IMMEDIATE_MASK);
		} else {
			runtime->pushData(stringAddr);
			runtime->pushData(0);
		}
		break; }
	case Colon:	{	// compiling word
		assert((*runtime)->compilerFlags == 0);
		
		// set state to compile
		(*runtime)->compilerFlags = -1;
		
		// -- create a new word --
		long newWordAddr = (*runtime)->allocate(sizeof(DictionaryWord) - sizeof(DictionaryWord::name));
		DictionaryWord* newWordPtr = (DictionaryWord*)newWordAddr;
		
		// set previous link
		newWordPtr->previous = (*runtime)->lastWord;
		(*runtime)->lastWord = newWordAddr;
		
		// clear all flags
		newWordPtr->flags = 0;
		
		// this is a colon word
		newWordPtr->opcode = Colon;
		
		// call Word to copy into memory where "name" field will be allocated.
		OpCode(OpCode::Word).execute(runtime);
		
		// allocate 32 bytes for the name field which will include the memory set by Word
		(*runtime)->allocate(sizeof(DictionaryWord::name));
		break; }
	case SemiColon:
	case Create:
	case SemiCode:
	case Constant:
	case Interpret:
	default: {
		break; }
	}
}


