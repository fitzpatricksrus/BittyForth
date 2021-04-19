//
#include <iostream>

#include "Runtime.hpp"
#include "Word.hpp"
#include "OpCode.hpp"
#include "Builder.hpp"

int bootstrap(int argc, const char * argv[]) {
	Runtime runtime(16384);
	Builder builder(&runtime);

	// dp needs to be somewhere that Builder can find it - the first thing in memory
	Word* w_dp = builder.allocateWord("dp", &Variable::doVar, sizeof(int));
	(*w_dp)[0] = 0;
	
//        variable 'state',state,0
	Word* w_state = builder.allocateWord("state", &Variable::doVar, sizeof(int));
	(*w_state)[0] = 0;
	
	Word* w_toIn = builder.allocateWord(">in", &Variable::doVar, sizeof(int));
	(*w_toIn)[0] = 0;
	
	Word* hashTib = builder.allocateWord("#tib", &Variable::doVar, sizeof(int));
	(*hashTib)[0] = 0;
	
	// last needs to be set when everything is done
	builder.allocateWord("last", &Variable::doVar, sizeof(int));
	
	builder.allocateWord("tib", &Variable::doVar, 256);
	
	builder.allocateWord("abort", &Abort::doAbort);
	Word* w_at = builder.allocateWord("@", &At::doAt);
	Word* w_put = builder.allocateWord("!", &Put::doPut);
	Word* w_lit = builder.allocateWord("(lit)", &Lit::doLit);
	Word* w_plus = builder.allocateWord("+", &Lit::doLit);
	Word* w_exit = builder.allocateWord("(exit)", &Exit::doExit);
	
	// : , { value -- } dp @ ! dp @ sizeof(int) + dp ! ;
	builder.allocateWord(",", &Colon::doColon);
	builder.append(w_dp);
	builder.append(w_at);
	builder.append(w_put);
	builder.append(w_dp);
	builder.append(w_at);
	builder.append(w_lit);
	builder.append(sizeof(int));
	builder.append(w_plus);
	builder.append(w_dp);
	builder.append(w_put);
	builder.append(w_exit);
	
	
	
	return 0;
}

#include <iostream>

int main(int argc, const char * argv[]) {
	std::cout << sizeof(int) << std::endl;
	return 0;
}
