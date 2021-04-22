//
//  Builder.cpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#include "Builder.hpp"

#include "Runtime.hpp"
#include "OpCode.hpp"
#include "DictionaryWord.hpp"
#include "CountedString.hpp"

Builder::Builder(Runtime* runtimeIn) {
	runtime = runtimeIn;
	marksIdx = 0;
}


std::string Builder::getNextInputWord(char delimeter) {
	std::string result;
	char* tibAddr = runtime->tibAddr;
	char* inputAddr = tibAddr + runtime->tibInputOffset;
	char* inputEndAddr = tibAddr + runtime->tibContentLength;
	
	//skip leading delimeters
	while ((inputAddr < inputEndAddr) && (*inputAddr == delimeter)) {
		inputAddr++;
	}
	
	while ((inputAddr < inputEndAddr) && (*inputAddr != delimeter)) {
		result += *inputAddr;
		inputAddr++;
	}
	
	runtime->tibInputOffset += result.length();
	
	return result;
}

DictionaryWord* Builder::findWord(const std::string& name) {
	DictionaryWord* word = runtime->lastWord;
	char nameCountedString[32];
	CountedString::fromCString(name, nameCountedString, sizeof(nameCountedString));
	bool done = false;
	while (word != 0 && !done) {
		if (CountedString::compare(nameCountedString, word->name) != 0) {
			word = word->previous;
		} else {
			done = true;
		}
	}
	return word;
}

DictionaryWord* Builder::createWord(OpCode opcode, Num flags) {
	return createWord(getNextInputWord(' '), opcode, flags);
}

DictionaryWord* Builder::createWord(const std::string& name, OpCode opcode, Num flags) {
	DictionaryWord* result = (DictionaryWord*)runtime->allocate(sizeof(DictionaryWord));
	result->previous = runtime->lastWord;
	runtime->lastWord = result;
	result->flags = flags;
	result->opcode = opcode;
	CountedString::fromCString(name, result->name, sizeof(result->name));
	return result;
}

XPtr Builder::append(const XData& data) {
	XData* result = (XData*)runtime->allocate(sizeof(data));
	*result = data;
	return (XPtr)result;
}

XPtr Builder::append(const std::string &wordName) {
	return append(findWord(wordName));
}

void Builder::compileBegin() {
	pushMark((XPtr)runtime->dictionaryPtr);
}

void Builder::compileIf() {
	append("0branch");
	pushMark(append((XPtr)nullptr));
}

void Builder::compileElse() {
	XPtr ifMark = popMark();
	append("branch");
	pushMark(append((XPtr)nullptr));
	*ifMark = runtime->dictionaryPtr;
}

void Builder::compileEndif() {
	XPtr ifMark = popMark();
	*ifMark = runtime->dictionaryPtr;
}

void Builder::compileAgain() {
	append("branch");
	append(popMark());
}

void Builder::pushMark() {
	pushMark((XPtr)runtime->dictionaryPtr);
}

void Builder::pushMark(XPtr m) {
	marks[marksIdx++] = m;
}

XPtr Builder::popMark() {
	return marks[--marksIdx];
}

	/*
		: interpret
		begin	//0
			#tib @ >in @ =
			if  //1
				tib 50 accept #tib ! 0 >in !
			then //2
			32 word find dup
			if	//3
				state @ =
				if	//4
					,
				else	//5
					execute
				then	//6
			else	//7
				dup rot count >number
				if	//8
					state @
					if	//9
						last @ dup @ last ! dp !
					then	//10
					abort
				then	//11
				drop drop state @
				if	//12
					['] lit , ,
				then	//13
			then	//14
		again	//15
		;
*/
