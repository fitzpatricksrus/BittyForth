#include <iostream>

/*
int bootstrap(int argc, const char * argv[]) {
	Runtime runtime(16384);
	Builder builder(&runtime);

	// dp needs to be somewhere that Builder can find it - the first thing in memory
	DictionaryWord* w_dp = builder.allocateWord("dp", &Variable::doVar, sizeof(int));
	(*w_dp)[0] = 0;
	
//        variable 'state',state,0
	DictionaryWord* w_state = builder.allocateWord("state", &Variable::doVar, sizeof(int));
	(*w_state)[0] = 0;
	
	DictionaryWord* w_toIn = builder.allocateWord(">in", &Variable::doVar, sizeof(int));
	(*w_toIn)[0] = 0;
	
	DictionaryWord* hashTib = builder.allocateWord("#tib", &Variable::doVar, sizeof(int));
	(*hashTib)[0] = 0;
	
	// last needs to be set when everything is done
	builder.allocateWord("last", &Variable::doVar, sizeof(int));
	
	builder.allocateWord("tib", &Variable::doVar, 256);
	
	builder.allocateWord("abort", &Abort::doAbort);
	DictionaryWord* w_at = builder.allocateWord("@", &At::doAt);
	DictionaryWord* w_put = builder.allocateWord("!", &Put::doPut);
	DictionaryWord* w_lit = builder.allocateWord("(lit)", &Lit::doLit);
	DictionaryWord* w_plus = builder.allocateWord("+", &Lit::doLit);
	DictionaryWord* w_exit = builder.allocateWord("(exit)", &Exit::doExit);
	
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
*/

int main(int argc, const char * argv[]) {
	std::cout << "int=" << sizeof(int) << " ptr=" << sizeof(void*) << " long=" << sizeof(long) << std::endl;
	return 0;
}
