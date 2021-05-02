//
//  InterpreterBuilder.cpp
//  ItsyForth
//
//  Created by Dad on 5/1/21.
//

#include "InterpreterBuilder.hpp"
#include "DictionaryWord.hpp"

InterpreterBuilder::InterpreterBuilder(Runtime* runtimeIn)
: runtime(runtimeIn), compiler(runtime)
{
}

DictionaryWord* InterpreterBuilder::create(const std::string& name, const Instruction& refIns) {
	int addr = compiler.compileWord(name);
	DictionaryWord* word = (DictionaryWord*)runtime->asPtr(addr);
	word->referenceInstruction = refIns;
}

	
void InterpreterBuilder::reset() {
	compiler.reset();
	
	// variables for data defined by compiler
	compiler.compileWord("dp", OpCode::DoLit, compiler.getDictionaryPtrAddr());
	compiler.compileWord("last", OpCode::DoLit, compiler.getLastWordPtrAddr());
	
	// words for basic opcodes
	
	compiler.compileWord("lit", OpCode::DoLit);
	compiler.compileWord(",", OpCode::Comma);
	compiler.compileWord("rot", OpCode::Rot);
	compiler.compileWord("drop", OpCode::Drop);
	compiler.compileWord("dup", OpCode::Dup);
	compiler.compileWord("swap", OpCode::Swap);
	compiler.compileWord("+", OpCode::Plus);
	compiler.compileWord("=", OpCode::Equals);
	compiler.compileWord("@", OpCode::At);
	compiler.compileWord("!", OpCode::Put);
	compiler.compileWord("0branch", OpCode::ZeroBranch);
	compiler.compileWord("branch", OpCode::Branch);
	compiler.compileWord("execute", OpCode::Execute);
	compiler.compileWord("exit", OpCode::Exit);
	compiler.compileWord("count", OpCode::Count);
	compiler.compileWord(">number", OpCode::ToNumber);
	compiler.compileWord("accept", OpCode::Accept);
	compiler.compileWord("word", OpCode::Word);
	compiler.compileWord("emit", OpCode::Emit);
	compiler.compileWord("find", OpCode::Find);
	compiler.compileWord("create", OpCode::Create);
	
	// allocate storage for compiler variables
	compiler.compileVariable("#tib", 0);
	compiler.compileVariable(">in", 0);
	compiler.compileVariable("state", 0);
	compiler.compileVariable("tibAddr", compiler.allocate(TIB_LENGTH));
	compiler.compileStartColonWord("tib");
		compiler.compileReference("tibAddr");
		compiler.compileReference("@");
		compiler.compileEndWord();
	compiler.compileStartColonWord("interpret");
		compiler.compileReference("#tib");
		compiler.compileReference("@");
		compiler.compileReference(">in");
		compiler.compileReference("@");
		compiler.compileReference("=");
		compiler.compileIf();
			compiler.compileReference("tib");
			compiler.compileLiteral(50);
			compiler.compileReference("accept");
			compiler.compileReference("#tib");
			compiler.compileReference("!");
			compiler.compileLiteral(0);
			compiler.compileReference(">in");
			compiler.compileReference("!");
		compiler.compileEndif();
		compiler.compileLiteral(32);
		compiler.compileReference("word");
		compiler.compileReference("find");
		compiler.compileReference("dup");
		compiler.compileIf();
			compiler.compileReference("state");
			compiler.compileReference("@");
			compiler.compileReference("=");
			compiler.compileIf();
				// do we need to >cfa here?
				compiler.compileReference(",");
			compiler.compileElse();
				compiler.compileReference("execute");
			compiler.compileEndif();
		compiler.compileElse();
			compiler.compileReference("dup");
			compiler.compileReference("rot");
			compiler.compileReference("count");
			compiler.compileReference(">number");
			compiler.compileIf();
				compiler.compileReference("state");
				compiler.compileReference("@");
				compiler.compileIf();
					compiler.compileReference("last");
					compiler.compileReference("@");
					compiler.compileReference("dup");
					compiler.compileReference("@");
					compiler.compileReference("last");
					compiler.compileReference("!");
					compiler.compileReference("dp");
					compiler.compileReference("!");
				compiler.compileEndif();
				compiler.compileReference("abort");
			compiler.compileEndif();
			compiler.compileReference("drop");
			compiler.compileReference("drop");
			compiler.compileReference("state");
			compiler.compileReference("@");
			compiler.compileIf();
				// compile TOS as a literal
//				compiler.compileReference("[']");
				compiler.compileReference("lit");
				compiler.compileReference(",");
				compiler.compileReference(",");
			compiler.compileEndif();
		compiler.compileEndif();
	compiler.compileAgain();
	
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
	 */
}
