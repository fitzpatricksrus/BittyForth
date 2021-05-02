//
//  Builder.cpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//
/*
#include "Builder.hpp"

#include "Runtime.hpp"
#include "OpCode.hpp"
#include "DictionaryWord.hpp"
#include "CountedString.hpp"
#include "Debug.hpp"

static const std::string INDENT("   ");

Builder::Builder(Runtime* runtimeIn) {
	runtime = runtimeIn;
	marksIdx = 0;
}

std::string Builder::getNextInputWord(char delimeter) {
	std::string result;
	char* tibAddr = runtime->getTibAddr();
	char* inputAddr = tibAddr + runtime->getTibInputOffset();
	char* inputEndAddr = tibAddr + runtime->getTibContentLength();
	
	//skip leading delimeters
	while ((inputAddr < inputEndAddr) && (*inputAddr == delimeter)) {
		inputAddr++;
	}
	
	while ((inputAddr < inputEndAddr) && (*inputAddr != delimeter)) {
		result += *inputAddr;
		inputAddr++;
	}
	
	runtime->setTibInputOffset(runtime->getTibInputOffset() + result.length());
	
	return result;
}

DictionaryWord* Builder::findWord(const OpCode& opcode) {
	return findWord(opcode.toString());
}

DictionaryWord* Builder::findWord(const std::string& name) {
	DictionaryWord* word = runtime->getLastWordPtr();
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
	if (word == nullptr) runtime->dbg("Word '"+name+"' not found in dictionary");
			// hey jf - throw an exception here
	return word;
}

DictionaryWord* Builder::createWord(OpCode opcode, Num flags) {
	return createWord(getNextInputWord(' '), opcode, flags);
}

DictionaryWord* Builder::createWord(const std::string& name, OpCode opcode, Num flags) {
	DictionaryWord* result = (DictionaryWord*)runtime->allocate(sizeof(DictionaryWord));
	result->previous = runtime->getLastWordPtr();
	runtime->setLastWordPtr(result);
	result->flags = flags;
	result->opcode = opcode;
	CountedString::fromCString(name, result->name, sizeof(result->name));
	
	runtime->dbg(result, opcode.toString() + " " + name);
	
	return result;
}

void Builder::compileOpCode(const OpCode& opcode) {
	compileReference(opcode.toString());
}

void Builder::compileConstant(const std::string& name, long value) {
	DictionaryWord* word = createWord(name, OpCode::Constant);
	append(value);
}

void Builder::compileLiteral(long value) {
	void* addr = append(OpCode::Lit);
	append(value);

	runtime->dbg(addr, OpCode(OpCode::Lit).toString() + " " + std::to_string(value));
}

void Builder::compileStartWord(const std::string& name, Num flags) {
	DictionaryWord* word = createWord(name, OpCode::Colon, flags);
	CountedString::fromCString(name, word->name, sizeof(word->name));
	
	runtime->dbg(word, OpCode(OpCode::Colon).toString() + " " + name);
}

void Builder::compileReference(DictionaryWord* word) {
	void* addr = append(word);
	
	runtime->dbg(addr, CountedString::toCString(word->name) + "(" + std::to_string((long)word) + ")");
}

void Builder::compileReference(const std::string& name) {
	DictionaryWord* word = findWord(name);
	append(word);

	runtime->dbg(word, name + "(" + std::to_string((long)word) + ")");
}

void Builder::compileEndWord() {
	compileReference(OpCode(OpCode::DoSemicolon));
}

void Builder::compileBegin() {
	pushMark((XData*)runtime->getDictionaryPtr());
}

void Builder::compileIf() {
	compileReference(OpCode(OpCode::ZeroBranch));
	pushMark(runtime->allocate(sizeof(XData)));
}

void Builder::compileElse() {
	XData* ifMark = popMark();
	compileReference(OpCode(OpCode::Branch));
	pushMark(runtime->allocate(sizeof(XData)));
	*ifMark = runtime->getDictionaryPtr();
}

void Builder::compileEndif() {
	XData* ifMark = popMark();
	*ifMark = runtime->getDictionaryPtr();
}

void Builder::compileAgain() {
	compileReference(OpCode(OpCode::Branch));
	append(popMark());
}

void Builder::pushMarkDP() {
	pushMark((XData*)runtime->getDictionaryPtr());
}

void Builder::pushMark(XData* m) {
	marks[marksIdx++] = m;
}

XData* Builder::popMark() {
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
*

XData* Builder::append(const XData& data) {
	XData* d = runtime->allocate(sizeof(XData));
	*d = data;
	return d;
}

void Builder::rebuildDictionary() {
	// recreate the system dictionary entries
	Builder builder(runtime);
	
	// system words
	builder.createWord("(doColon)", OpCode::DoColon);
	builder.createWord("(doSemicolon)", OpCode::DoSemicolon);
	builder.createWord("(doConstant)", OpCode::DoConstant);
	builder.createWord("(doVariable)", OpCode::DoVariable);
	builder.createWord("abort", OpCode::Abort);
	builder.createWord(",", OpCode::Comma);
	builder.createWord("(lit)", OpCode::Lit);
	builder.createWord("rot", OpCode::Rot);
	builder.createWord("drop", OpCode::Drop);
	builder.createWord("dup", OpCode::Dup);
	builder.createWord("swap", OpCode::Swap);
	builder.createWord("+", OpCode::Plus);
	builder.createWord("=", OpCode::Equals);
	builder.createWord("@", OpCode::At);
	builder.createWord("!", OpCode::Put);
	builder.createWord("(0branch)", OpCode::ZeroBranch);
	builder.createWord("(branch)", OpCode::Branch);
	builder.createWord("execute", OpCode::Execute);
	builder.createWord("exit", OpCode::Exit);
	builder.createWord("count", OpCode::Count);
	builder.createWord(">number", OpCode::ToNumber);
	builder.createWord("accept", OpCode::Accept);
	builder.createWord("word", OpCode::Word);
	builder.createWord("emit", OpCode::Emit);
	builder.createWord("find", OpCode::Find);
	builder.createWord(":", OpCode::Colon);
	builder.createWord(";", OpCode::SemiColon);
	builder.createWord("constant", OpCode::Constant);

	// system variables
	builder.createWord("abortWord", OpCode::Constant);
	builder.append(runtime->getAbortWordPtrAddr());
	builder.createWord("dp", OpCode::Constant);
	builder.append(runtime->getDictionaryPtrAddr());
	builder.createWord("base", OpCode::Constant);
	builder.append(runtime->getNumberBaseAddr());
	builder.createWord("tibAddr", OpCode::Constant);
	builder.append(runtime->getTibAddrAddr());
	builder.createWord("tib", OpCode::Colon);
		builder.compileReference("tibAddr");
		builder.compileReference("@");
		builder.compileReference(";");
	builder.createWord("#tib", OpCode::Constant);
	builder.append(runtime->getTibContentLengthAddr());
	builder.createWord(">in", OpCode::Constant);
	builder.append(runtime->getTibInputOffsetAddr());
	builder.createWord("lastWord", OpCode::Constant);
	builder.append(runtime->getLastWordPtrAddr());
	builder.createWord("compilerFlags", OpCode::Constant);
	builder.append(runtime->getCompilerFlagsAddr());
	
	/*
		: interpret
		begin
			#tib @ >in @ =
			if
				tib 50 accept #tib ! 0 >in !
			then
			32 word find dup
			if
				state @ =
				if
					,
				else
					execute
				then
			else
				dup rot count >number
				if
					state @
					if
						last @ dup @ last ! dp !
					then
					abort
				then
				drop drop state @
				if
					['] lit , ,
				then
			then
		again
		;
	 *
	builder.createWord("interpret", OpCode::Colon);
	builder.compileBegin();
		builder.compileReference("#tib");
		builder.compileReference("@");
		builder.compileReference(">in");
		builder.compileReference("@");
		builder.compileReference("=");
		builder.compileIf();
			builder.compileReference("tib");
			builder.compileLiteral(50);
			builder.compileReference("accept");
			builder.compileReference("#tib");
			builder.compileReference("!");
			builder.compileReference(">in");
			builder.compileReference("!");
		builder.compileEndif();
		builder.compileLiteral(32);
		builder.compileReference("word");
		builder.compileReference("find");
		builder.compileReference("dup");
		builder.compileIf();
			builder.compileReference("state");
			builder.compileReference("@");
			builder.compileReference("=");
			builder.compileIf();
				builder.compileReference(",");
			builder.compileElse();
				builder.compileReference("execute");
			builder.compileEndif();
		builder.compileElse();
			builder.compileReference("dup");
			builder.compileReference("rot");
			builder.compileReference("count");
			builder.compileReference(">number");
			builder.compileIf();
				builder.compileReference("state");
				builder.compileReference("@");
				builder.compileIf();
					builder.compileReference("last");
					builder.compileReference("@");
					builder.compileReference("dup");
					builder.compileReference("@");
					builder.compileReference("last");
					builder.compileReference("!");
					builder.compileReference("dp");
					builder.compileReference("!");
				builder.compileEndif();
				builder.compileReference("abort");
			builder.compileEndif();
			builder.compileReference("drop");
			builder.compileReference("drop");
			builder.compileReference("state");
			builder.compileReference("@");
			builder.compileReference("tib");
			builder.compileIf();
				builder.compileReference("(lit)");
				builder.compileReference("(lit)");
				builder.compileReference(",");
				builder.compileReference(",");
			builder.compileEndif();
		builder.compileEndif();
	builder.compileAgain();
	builder.compileReference("(doSemicolon)");

}
*/
