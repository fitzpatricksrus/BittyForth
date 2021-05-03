#include "CoreDictionary.hpp"

#include "Runtime.hpp"
#include "Builder.hpp"
#include "OpCode.hpp"

CoreDictionary::CoreDictionary(Runtime* runtimeIn) {
	runtime = runtimeIn;
}

DictionaryWord* CoreDictionary::find(const std::string& name) {
	return Builder(runtime).findWord(name);
}

void CoreDictionary::rebuildDictionary() {
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
		builder.append("tibAddr");
		builder.append("@");
		builder.append(";");
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
	 */
	builder.createWord("interpret", OpCode::Colon);
	builder.compileBegin();
		builder.append("#tib");
		builder.append("@");
		builder.append(">in");
		builder.append("@");
		builder.append("=");
		builder.compileIf();
			builder.append("tib");
			builder.append("(lit)");
			builder.append(50);
			builder.append("accept");
			builder.append("#tib");
			builder.append("!");
			builder.append(">in");
			builder.append("!");
		builder.compileEndif();
		builder.append("(lit)");
		builder.append(32);
		builder.append("word");
		builder.append("find");
		builder.append("dup");
		builder.compileIf();
			builder.append("state");
			builder.append("@");
			builder.append("=");
			builder.compileIf();
				builder.append(",");
			builder.compileElse();
				builder.append("execute");
			builder.compileEndif();
		builder.compileElse();
			builder.append("dup");
			builder.append("rot");
			builder.append("count");
			builder.append(">number");
			builder.compileIf();
				builder.append("state");
				builder.append("@");
				builder.compileIf();
					builder.append("last");
					builder.append("@");
					builder.append("dup");
					builder.append("@");
					builder.append("last");
					builder.append("!");
					builder.append("dp");
					builder.append("!");
				builder.compileEndif();
				builder.append("abort");
			builder.compileEndif();
			builder.append("drop");
			builder.append("drop");
			builder.append("state");
			builder.append("@");
			builder.append("tib");
			builder.compileIf();
				builder.append("(lit)");
				builder.append("(lit)");
				builder.append(",");
				builder.append(",");
			builder.compileEndif();
		builder.compileEndif();
	builder.compileAgain();
	builder.append("(doSemicolon)");

}
