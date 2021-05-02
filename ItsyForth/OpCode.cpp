#include "OpCode.hpp"



/*

#include "Runtime.hpp"
#include "DictionaryWord.hpp"
#include "CountedString.hpp"
#include "StructuredMemory.hpp"
#include "Builder.hpp"
#include <string>
#include <iostream>
#include "TermialIO.hpp"

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

OpCode::OpCode(OpCode::Code codeIn)
: code(codeIn)
{
}

OpCode::operator std::string() const {
	return toString();
}

std::string OpCode::toString() const {
	static const std::string names[] = {
		"(colon)",
		"(semicolon)",
		"(constant)",
		"(variable)",
		"abort",
		",",
		"(lit)",
		"rot",
		"drop",
		"dup",
		"swap",
		"+",
		"=",
		"@",
		"!",
		"(0branch)",
		"(branh)",
		"execute",
		"exit",
		"count",
		">number",
		"accept",
		"word",
		"emit",
		"find",
		";",
		":",
		"create",
		"constant"
	};
	return names[code];
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
		Builder(runtime).compileReference(runtime->popData());
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
		int base = runtime->getNumberBase();
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
		int maxLen = runtime->popData();
		char* addr = (char*)runtime->popData();
		long ndx;
		do {
			char c = runtime->read();
			for (ndx = 0; ndx < maxLen && c != '\n'; ndx++) {
				addr[ndx] = c;
				c = runtime->read();
			}
		} while (ndx == 0);	//skip empty lines
//		if (ndx >= maxLen) {
			// hey jf - this overflowed the input buffer
			// do you want to print a message on the
			// terminal and flush the remaining input?
//		}
		runtime->pushData(ndx);
		break; }
	case Word: { // ( char -- addr )
		char* pad = (char*)runtime->getDictionaryPtr();
		char delimeter = runtime->popData();
		std::string nameIn = Builder(runtime).getNextInputWord(delimeter);
		CountedString::fromCString(nameIn, pad, padLength);
		runtime->pushData(pad);
		break; }
	case Emit:	// ( char -- )
		runtime->emit((char)runtime->popData());
		break;
	case Find: {
		// addr -- addr2 flag  look up word in the dictionary
		// find looks in the Forth dictionary for a word with the name given in the
		// counted string at addr. One of the following will be returned:
		// flag =  0, addr2 = counted string --> word was not found
		// flag =  1, addr2 = call address   --> word is immediate
		// flag = -1, addr2 = call address   --> word is not immediate
		char* stringAddr = runtime->popData();
		DictionaryWord* thisWordAddr = runtime->getLastWordPtr();
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
		assert(runtime->getCompilerFlags() == 0);
		// set state to compile
		runtime->setCompilerFlags(-1);
		Builder(runtime).createWord(OpCode::Colon);
		break; }
	case Create: {
		// -- create a new word --
		Builder(runtime).createWord(OpCode::Colon);
		break; }
	default: {
		break; }
	}
}
*/

